import React, { useRef, useEffect, useState, useCallback } from 'react';
import * as THREE from 'three';
import { Heartbeat } from './components/heartbeat';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js'; // Corrected import path
import StateButton from './components/state-button';

const App = () => {
  const canvasRef = useRef(null);
  const threeRefs = useRef({
    scene: null,
    camera: null,
    renderer: null,
    rocket: null,
    onWindowResize: null,
    animationFrameId: null,
    pivot: null,
  });

  const [orientation, setOrientation] = useState({ roll: 0, pitch: 0, yaw: 0 });
  const [heartbeat, setHeartbeat] = useState({ status: -1, timestamp: 0 });

  const CANVAS_WIDTH = 512;

  const initThreeScene = useCallback(() => {
    if (!canvasRef.current) {
      console.error('Canvas element not found.');
      return;
    }

    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x282c34);

    const camera = new THREE.PerspectiveCamera(75, CANVAS_WIDTH / window.innerHeight, 0.1, 1000);
    camera.position.set(0, 2, 5);

    const renderer = new THREE.WebGLRenderer({ canvas: canvasRef.current, antialias: true });
    renderer.setSize(CANVAS_WIDTH, window.innerHeight);
    renderer.setPixelRatio(window.devicePixelRatio);

    const onWindowResize = () => {
      const currentWidth = canvasRef.current.clientWidth;
      const currentHeight = canvasRef.current.clientHeight;

      camera.aspect = currentWidth / currentHeight;
      camera.updateProjectionMatrix();
      renderer.setSize(currentWidth, currentHeight);
      console.log(`Canvas resized to: ${currentWidth}x${currentHeight}`);
    };
    window.addEventListener('resize', onWindowResize);
    onWindowResize();

    const ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
    scene.add(ambientLight);
    const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
    directionalLight.position.set(0, 5, 5).normalize();
    scene.add(directionalLight);

    const loader = new GLTFLoader();
    loader.load('/falcon.glb',
      (gltf) => {
        const pivot = new THREE.Object3D();

        const rocketModel = gltf.scene;
        rocketModel.scale.set(0.08, 0.08, 0.08);
        rocketModel.position.set(0, -2, 0);

        pivot.add(rocketModel);

        pivot.position.set(0, 1, 0);

        threeRefs.current.rocket = rocketModel;
        threeRefs.current.pivot = pivot;

        console.log('Model loaded successfully:', rocketModel);

        scene.add(pivot);
      },
      (xhr) => {
        console.log((xhr.loaded / xhr.total * 100) + '% loaded');
      },
      (error) => {
        console.error('An error occurred while loading the model:', error);
      }
    );

    threeRefs.current.scene = scene;
    threeRefs.current.camera = camera;
    threeRefs.current.renderer = renderer;
    threeRefs.current.onWindowResize = onWindowResize;
  }, []);

  useEffect(() => {
    initThreeScene();

    const disposeHierarchy = (node) => {
        node.traverse((child) => {
          if (child.geometry) child.geometry.dispose();
          if (child.material) {
            if (Array.isArray(child.material)) {
              child.material.forEach((mat) => mat.dispose());
            } else {
              child.material.dispose();
            }
          }
        });
      };
      
      return () => {
        if (threeRefs.current.renderer) {
          threeRefs.current.renderer.dispose();
        }
      
        if (threeRefs.current.onWindowResize) {
          window.removeEventListener('resize', threeRefs.current.onWindowResize);
        }
      
        if (threeRefs.current.pivot) {
          disposeHierarchy(threeRefs.current.pivot);
        }
      
        if (threeRefs.current.animationFrameId) {
          cancelAnimationFrame(threeRefs.current.animationFrameId);
        }
      };
    }, [initThreeScene]);

  useEffect(() => {
    const animate = () => {
      const { scene, camera, renderer, pivot } = threeRefs.current;

      if (scene && camera && renderer) {
        if (pivot) {
          pivot.rotation.x = orientation.pitch;
          pivot.rotation.y = orientation.yaw;
          pivot.rotation.z = orientation.roll;
        }
        renderer.render(scene, camera);
      }
      threeRefs.current.animationFrameId = requestAnimationFrame(animate);
    };

    animate();

    return () => {
        if (threeRefs.current.animationFrameId) {
          cancelAnimationFrame(threeRefs.current.animationFrameId);
        }
      };
  }, [orientation]);

  const ws = useRef(null);
  useEffect(() => {
    ws.current = new WebSocket('ws://192.168.2.194:8080/ws');

    ws.current.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        const json = JSON.parse(data.data);

        switch(json.packet_type) {
          case 4: // Heartbeat packet
            setHeartbeat({ status: json.payload.status, timestamp: data.timestamp });
            break;
          case 3: // Telemetry packet
            setOrientation({
                roll: json.payload.roll * Math.PI / 180,
                pitch: json.payload.pitch * Math.PI / 180,
                yaw: json.payload.yaw * Math.PI / 180,
            });
            break;
          default:
            console.log('Unknown packet type:', json.packet_type);
        }
      } catch (e) {
        console.error('Error parsing WebSocket message:', e, event.data);
      }
    };

    ws.current.onopen = () => console.log('WebSocket connected');
    ws.current.onerror = (error) => console.error('WebSocket error:', error);
    ws.current.onclose = () => console.log('WebSocket disconnected');

    return () => {
        ws.current.close();
        console.log('WebSocket closed.');
    };
  }, []);

    return (
        <div className="w-screen h-screen flex flex-row relative">
            <div style={{ width: CANVAS_WIDTH, height: '100vh' }} className='relative'>
                <Heartbeat heartbeat={heartbeat} />
                <canvas ref={canvasRef} className="w-full h-full"></canvas>
                <div className='absolute top-0 right-0'>
                    <p>Pitch: {(orientation.pitch * 180 / Math.PI).toFixed(2)}°</p>
                    <p>Roll: {(orientation.roll * 180 / Math.PI).toFixed(2)}°</p>
                    <p>Yaw: {(orientation.yaw * 180 / Math.PI).toFixed(2)}°</p>
                </div>
            </div>
            <div className='w-full h-full pl-2'>
                <div className='flex flex-col w-full'>
                    <h2 className='text-2xl'>State</h2>
                    <div className='flex flex-row'>
                        <StateButton state={0} title={"INIT"} currentState={heartbeat.status} onClick={() => {ws.current.send("state 0")}} />
                        <StateButton state={1} title={"READY"} currentState={heartbeat.status} onClick={() => {ws.current.send("state 1")}} />
                        <StateButton state={2} title={"ARMED"} currentState={heartbeat.status} onClick={() => {ws.current.send("state 2")}} />
                        <StateButton state={3} title={"IN_FLIGHT"} currentState={heartbeat.status} />
                        <StateButton state={4} title={"GLIDING"} currentState={heartbeat.status} />
                        <StateButton state={5} title={"LANDED"} currentState={heartbeat.status} />
                    </div>
                </div>
                <div className=''>
                    <h2 className='text-2xl'>Altitude</h2>
                    
                </div>
            </div>
        </div>
    );
};
export default App;
