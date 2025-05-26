#!/bin/bash

INPUT_LOG="../raw/ground-station-raw.txt"
OUTPUT_CSV="../raw/ground-station.CSV"

echo "pressure,temperature,altitude,ax,ay,az,pitch,roll,yaw" > "$OUTPUT_CSV"

grep '"packet_type":3' "$INPUT_LOG" | while IFS= read -r line; do
    payload=$(echo "$line" | sed -E 's/.*"payload":({.*}).*/\1/' | sed 's/}}$//')

    pressure=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"pressure":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    temperature=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"temperature":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    
    altitude=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"altitude":/) {split($i, a, ":"); print a[2]}}' | tr -d '"}' | sed 's/[^0-9.]*$//')

    ax=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"ax":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    ay=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"ay":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    az=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"az":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    pitch=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"pitch":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    roll=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"roll":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')
    yaw=$(echo "$payload" | awk -F',' '{for(i=1;i<=NF;i++) if($i ~ /"yaw":/) {split($i, a, ":"); print a[2]}}' | tr -d '"')

    echo "$pressure,$temperature,$altitude,$ax,$ay,$az,$pitch,$roll,$yaw" >> "$OUTPUT_CSV"

done

echo "Data extracted to $OUTPUT_CSV"
