import React, { useRef, useEffect, useState, useCallback } from 'react';
import * as THREE from 'three';
import CenteredLineChart from './components/CenteredLineChart';
// import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader';

const App = () => {
  const canvasRef = useRef(null);
  const threeRefs = useRef({});
  const [orientation, setOrientation] = useState({ roll: 0, pitch: 0, yaw: 0 });
  const [message, setMessage] = useState('Initializing 3D scene...');

  const CANVAS_WIDTH = 512;
  const CANVAS_HEIGHT = window.innerHeight;

  const initThreeScene = useCallback(() => {
    if (!canvasRef.current) return;

    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x282c34);

    const camera = new THREE.PerspectiveCamera(75, CANVAS_WIDTH / CANVAS_HEIGHT, 0.1, 1000);
    camera.position.z = 5;

    const renderer = new THREE.WebGLRenderer({ canvas: canvasRef.current, antialias: true });
    renderer.setSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    renderer.setPixelRatio(window.devicePixelRatio);

    // resizing
    const onWindowResize = () => {
      camera.aspect = CANVAS_WIDTH / CANVAS_HEIGHT;
      camera.updateProjectionMatrix();
      renderer.setSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    };
    window.addEventListener('resize', onWindowResize);

    const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
    scene.add(ambientLight);
    const directionalLight = new THREE.DirectionalLight(0xffffff, 0.8);
    directionalLight.position.set(0, 5, 5).normalize();
    scene.add(directionalLight);

    // const loader = new GLTFLoader();
    // loader.load('rocket.glb', (gltf) => {
    //   const rocketModel = gltf.scene;
    //   rocketModel.scale.set(0.5, 0.5, 0.5); //scale
    //   scene.add(rocketModel);
    //   threeRefs.current.rocket = rocketModel; // ref
    //   setMessage('Rocket model loaded. Simulating data...');
    // }, undefined, (error) => {
    //   console.error('An error occurred while loading the model:', error);
    //   setMessage('Error loading rocket model.');
    // });

    // Cylinder (pklacehold for rkt)
    const geometry = new THREE.CylinderGeometry(0.5, 0.5, 2, 32); // top, bot, hei, segf
    const material = new THREE.MeshPhongMaterial({ color: 0x007bff });
    const rocketMesh = new THREE.Mesh(geometry, material);
    scene.add(rocketMesh);
    rocketMesh.rotation.order = 'YXZ';
    threeRefs.current = { scene, camera, renderer, rocket: rocketMesh, onWindowResize };

    setMessage('3D scene initialized. Simulating data...');
  }, []);

  useEffect(() => {
    initThreeScene();

    return () => {
      if (threeRefs.current.renderer) {
        threeRefs.current.renderer.dispose();
      }
      if (threeRefs.current.onWindowResize) {
        window.removeEventListener('resize', threeRefs.current.onWindowResize);
      }
      if (threeRefs.current.rocket && threeRefs.current.rocket.geometry) {
        threeRefs.current.rocket.geometry.dispose();
      }
      if (threeRefs.current.rocket && threeRefs.current.rocket.material) {
        threeRefs.current.rocket.material.dispose();
      }
    };
  }, [initThreeScene]);

  useEffect(() => {
    const { scene, camera, renderer, rocket } = threeRefs.current;
    if (!scene || !camera || !renderer || !rocket) return;

    let animationFrameId;
    let wsSimulationInterval;

    // const ws = new WebSocket('ws://localhost:8080');
    // ws.onmessage = (event) => {
    //   const data = JSON.parse(event.data);
    //   setOrientation({
    //     roll: data.roll,
    //     pitch: data.pitch,
    //     yaw: data.yaw,
    //   });
    // };
    // ws.onopen = () => console.log('WebSocket connected');
    // ws.onerror = (error) => console.error('WebSocket error:', error);
    // ws.onclose = () => console.log('WebSocket disconnected');

    // fake data
    wsSimulationInterval = setInterval(() => {
      const newRoll = (Math.random() - 0.5) * Math.PI * 2; // -PI to PI
      const newPitch = (Math.random() - 0.5) * Math.PI / 2; // -PI/4 to PI/4
      const newYaw = (Math.random() - 0.5) * Math.PI * 2; // -PI to PI
      setOrientation({
        roll: newRoll,
        pitch: newPitch,
        yaw: newYaw,
      });
      setMessage('Receiving simulated data...');
    }, 100);

    const animate = () => {
      rocket.rotation.set(orientation.pitch, orientation.yaw, orientation.roll);

      renderer.render(scene, camera);
      animationFrameId = requestAnimationFrame(animate);
    };

    animate();

    return () => {
      cancelAnimationFrame(animationFrameId);
      clearInterval(wsSimulationInterval);
    };
  }, [orientation]);

  const [data, setData] = useState([]);
  useEffect(() => {
    setInterval(() => {
        const { roll, pitch, yaw } = GenerateFakeOrientationData();
        setData(prevData => {
            const newData = [...prevData, { x: Date.now(), y: roll }];
            if (newData.length > 20) {
                newData.shift(); // remove the oldest data point
            }
            return newData;
        });
    }, 200);
  }, []);

  return (
    <div className="w-screen h-screen flex flex-row">
      {/* canv for Threej */}
        <div style={{ width: CANVAS_WIDTH, height: CANVAS_HEIGHT }}>
            <canvas ref={canvasRef} className="w-full h-full"></canvas>
        </div>
        <div className='w-full h-full'>
            <div className='p-2'>
                <h1>Telemetry</h1>
                <div className='flex flex-col h-80 gap-2'>
                <div className='h-1/3'>
                <CenteredLineChart data={data} title='row'/>
                </div>
                <div className='h-1/3'>
                <CenteredLineChart data={data} title='pitch'/>
                </div>
                <div className='h-1/3'>
                <CenteredLineChart data={data} title='yaw'/>
                </div>
                </div>
            </div>
        </div>
    </div>
  );
};

function GenerateFakeOrientationData() {
    const roll = (Math.random() - 0.5) * Math.PI * 2; // Random roll between -PI and PI
    const pitch = (Math.random() - 0.5) * Math.PI / 2; // Random pitch between -PI/4 and PI/4
    const yaw = (Math.random() - 0.5) * Math.PI * 2; // Random yaw between -PI and PI
    return { roll, pitch, yaw };
}

export default App;
