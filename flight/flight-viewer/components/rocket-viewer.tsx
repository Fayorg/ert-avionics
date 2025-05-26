import { useCallback, useEffect, useRef } from 'react';
import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';

export default function RocketViewer({ orientation }: { orientation: { roll: number; pitch: number; yaw: number } }) {
	const canvasRef = useRef<HTMLCanvasElement>(null);
	const threeRefs = useRef({
		scene: null,
		camera: null,
		renderer: null,
		rocket: null,
		animationFrameId: null,
		pivot: null,
		onWindowResize: () => {},
	});

	const CANVAS_WIDTH = canvasRef.current?.width || 512;

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
			const currentWidth = canvasRef.current?.clientWidth || CANVAS_WIDTH;
			const currentHeight = canvasRef.current?.clientHeight || window.innerHeight;

			camera.aspect = currentWidth / currentHeight;
			camera.updateProjectionMatrix();
			renderer.setSize(currentWidth, currentHeight);
		};
		window.addEventListener('resize', onWindowResize);
		onWindowResize();

		const ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
		scene.add(ambientLight);
		const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
		directionalLight.position.set(0, 5, 5).normalize();
		scene.add(directionalLight);

		const loader = new GLTFLoader();
		loader.load(
			'/falcon.glb',
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
				console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
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
				(threeRefs.current.renderer as any).dispose();
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
					(pivot as any).rotation.x = orientation.pitch * (Math.PI / 180);
					(pivot as any).rotation.y = orientation.yaw * (Math.PI / 180);
					(pivot as any).rotation.z = orientation.roll * (Math.PI / 180);
				}
				(renderer as any).render(scene, camera);
			}
			(threeRefs.current.animationFrameId as any) = requestAnimationFrame(animate);
		};

		animate();

		return () => {
			if (threeRefs.current.animationFrameId) {
				cancelAnimationFrame(threeRefs.current.animationFrameId);
			}
		};
	}, [orientation]);

	return (
		<div>
			<canvas ref={canvasRef} className="w-full h-full"></canvas>
		</div>
	);
}
