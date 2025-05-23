package main

import (
	"bufio"
	"encoding/json" // New import for JSON handling
	"io"
	"log"
	"net/http"
	"os"
	"strings" // New import for string manipulation
	"sync"
	"time"

	"github.com/gorilla/websocket" // For WebSocket communication
	"github.com/tarm/serial"       // For serial port communication
)

const (
	// Replace with your COM port name (e.g., "COM1" on Windows, "/dev/ttyUSB0" on Linux/macOS)
	comPortName = "COM5"
	baudRate    = 115200 // Standard baud rate, adjust as needed

	// Path to the local file where data will be written
	outputFilePath = "logs/output.log"

	// Address for the WebSocket server to listen on (e.g., ":8080" for all interfaces on port 8080)
	websocketServerAddr = ":8080"

	// bufferSize is no longer directly used for reading lines, but kept for potential future use or context
	// bufferSize = 1024
)

// Define a WebSocket upgrader
var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
	CheckOrigin: func(r *http.Request) bool {
		// Allow all origins for simplicity in this example.
		// In a production environment, you should restrict this to known origins.
		return true
	},
}

// clients holds all currently connected WebSocket clients
var clients = make(map[*websocket.Conn]bool) // Connected clients
var clientsMutex = &sync.Mutex{}             // Mutex to protect concurrent access to clients map

var serialPort *serial.Port          // Global variable to hold the serial port instance
var serialWriteMutex = &sync.Mutex{} // Mutex to protect writes to the serial port from multiple goroutines

// ComData is the structure for the JSON object sent over WebSocket
type ComData struct {
	Timestamp string `json:"timestamp"` // Timestamp of when the data was received
	Data      string `json:"data"`      // The actual data content
}

// handleConnections manages individual WebSocket client connections.
// It upgrades the HTTP connection to a WebSocket and handles incoming messages from the client,
// forwarding them to the COM port.
func handleConnections(w http.ResponseWriter, r *http.Request) {
	// Upgrade the HTTP GET request to a WebSocket connection
	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Printf("Failed to upgrade connection: %v", err)
		return
	}

	// Ensure the WebSocket connection is closed when this function returns
	defer func() {
		log.Println("Client disconnected.")
		// Remove the client from the global map of active clients
		clientsMutex.Lock()
		delete(clients, ws)
		clientsMutex.Unlock()
		// Attempt to close the WebSocket connection
		if err := ws.Close(); err != nil {
			log.Printf("Error closing client WebSocket: %v", err)
		}
	}()

	// Add the new client to our map of active clients
	clientsMutex.Lock()
	clients[ws] = true
	clientsMutex.Unlock()
	log.Println("New client connected.")

	// Start a new goroutine to handle messages *from* this specific client.
	// This allows the server to concurrently handle multiple clients sending data.
	for {
		// Read a message from the WebSocket client.
		// messageType will be websocket.TextMessage or websocket.BinaryMessage.
		// p will contain the message payload.
		messageType, p, err := ws.ReadMessage()
		if err != nil {
			// Check if the error is a normal WebSocket close error
			if websocket.IsCloseError(err, websocket.CloseNormalClosure, websocket.CloseGoingAway) {
				log.Println("Client closed connection normally.")
			} else {
				log.Printf("Error reading from WebSocket client: %v", err)
			}
			break // Exit the loop on error or disconnect
		}

		// Process the received message based on its type
		if messageType == websocket.TextMessage || messageType == websocket.BinaryMessage {
			log.Printf("Received message from client (Type: %d): %s", messageType, p)

			// Check if the serial port is open before attempting to write
			if serialPort != nil {
				// Acquire a lock before writing to the serial port to prevent race conditions
				// if multiple clients try to write simultaneously.
				serialWriteMutex.Lock()
				n, err := serialPort.Write(p) // Write the received message bytes to the COM port
				serialWriteMutex.Unlock()     // Release the lock

				if err != nil {
					log.Printf("Error writing to COM port from WebSocket client: %v", err)
				} else {
					log.Printf("Wrote %d bytes to COM port from WebSocket client.", n)
				}
			} else {
				log.Println("Serial port not open, cannot write data from client.")
			}
		}
	}
}

func main() {
	log.Println("Starting COM port to WebSocket server forwarder...")

	// 1. Open Serial Port
	c := &serial.Config{Name: comPortName, Baud: baudRate, ReadTimeout: time.Second * 5}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatalf("Failed to open serial port %s: %v", comPortName, err)
	}
	serialPort = s // Assign the opened serial port to the global variable
	defer func() {
		log.Printf("Closing serial port %s...", comPortName)
		if err := serialPort.Close(); err != nil {
			log.Printf("Error closing serial port: %v", err)
		}
	}()
	log.Printf("Successfully opened serial port %s at %d baud.", comPortName, baudRate)

	// 2. Open Local File for Writing
	f, err := os.OpenFile(outputFilePath, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatalf("Failed to open output file %s: %v", outputFilePath, err)
	}
	defer func() {
		log.Printf("Closing output file %s...", outputFilePath)
		if err := f.Close(); err != nil {
			log.Printf("Error closing file: %v", err)
		}
	}()
	log.Printf("Successfully opened output file %s for appending.", outputFilePath)

	// Create a channel to pass data from the COM port reader to the WebSocket sender
	dataChan := make(chan []byte, 100) // Buffered channel for efficiency

	// Goroutine 1: Read from COM port line by line, filter, convert to JSON, and send to file and channel
	go func() {
		reader := bufio.NewReader(serialPort) // Create a new buffered reader for the global serial port
		for {
			line, err := reader.ReadString('\n') // Read until newline character
			if err != nil {
				if err == io.EOF {
					log.Println("COM port closed or EOF reached.")
					break
				}
				log.Printf("Error reading line from COM port: %v", err)
				time.Sleep(time.Second) // Wait a bit before retrying to avoid busy-looping on persistent errors
				continue
			}

			// Trim leading/trailing whitespace, including newline characters, from the line.
			trimmedLine := strings.TrimSpace(line)

			// --- Filtering and JSON Conversion Logic ---
			// Only process lines that start with '*'
			if strings.HasPrefix(trimmedLine, "*") {
				// Remove the leading '*' from the content
				content := strings.TrimPrefix(trimmedLine, "*")

				// Create a ComData struct with the current timestamp and the extracted content
				comData := ComData{
					Timestamp: time.Now().Format(time.RFC3339), // Format timestamp as RFC3339 string
					Data:      content,
				}

				// Marshal the ComData struct into a JSON byte slice
				jsonData, err := json.Marshal(comData)
				if err != nil {
					log.Printf("Error marshalling JSON: %v", err)
					continue // Skip this line if JSON conversion fails
				}

				log.Printf("Received filtered line from COM port, converted to JSON: %s", jsonData)

				// Write the original line (including newline) to the local file.
				// You could choose to write the JSON data here instead if preferred.
				if _, err := f.Write([]byte(line)); err != nil {
					log.Printf("Error writing original line to file: %v", err)
				} else {
					log.Println("Original line written to file.")
				}

				// Send the JSON byte slice to the data channel for WebSocket broadcasting
				select {
				case dataChan <- jsonData:
					// Data successfully sent to channel
				default:
					log.Println("Data channel is full, dropping JSON line.")
				}
			} else {
				log.Printf("Line did not start with '*': %q (not forwarded to WebSocket)", trimmedLine)
			}
		}
		close(dataChan) // Close the channel when the COM port reading goroutine finishes
	}()

	// Goroutine 2: Read from channel and broadcast to all connected WebSockets
	go func() {
		for data := range dataChan {
			clientsMutex.Lock() // Lock access to the clients map
			for client := range clients {
				log.Printf("Broadcasting %d bytes (JSON line) to WebSocket client...", len(data))
				// Send the JSON data as a TextMessage over the WebSocket
				if err := client.WriteMessage(websocket.TextMessage, data); err != nil {
					log.Printf("Error writing to WebSocket client: %v", err)
					// If there's an error writing, assume the client is disconnected or problematic.
					// Attempt to close the client's WebSocket connection and remove it from the map.
					if err := client.Close(); err != nil {
						log.Printf("Error closing problematic client WebSocket: %v", err)
					}
					delete(clients, client)
				} else {
					log.Println("JSON line broadcast to WebSocket client.")
				}
			}
			clientsMutex.Unlock() // Unlock access to the clients map
		}
		log.Println("WebSocket broadcaster goroutine finished.")
	}()

	// 3. Start WebSocket Server
	log.Printf("Starting WebSocket server on %s...", websocketServerAddr)
	// Register the handleConnections function to handle incoming WebSocket connections at the "/ws" endpoint
	http.HandleFunc("/ws", handleConnections)
	// Start the HTTP server and listen for connections. This call blocks indefinitely.
	err = http.ListenAndServe(websocketServerAddr, nil)
	if err != nil {
		log.Fatalf("Failed to start WebSocket server: %v", err)
	}
}
