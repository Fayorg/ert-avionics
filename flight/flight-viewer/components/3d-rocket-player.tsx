import { useEffect, useState, useRef, useCallback } from 'react';
import { loadDataFromCSV } from '../lib/load-csv';
import RocketViewer from './rocket-viewer';
import { ChevronLeft, ChevronRight, Expand, Info, Pause, Play, RotateCcw } from 'lucide-react';
import { cn } from '@sglara/cn';

export default function RocketPlayer({ path = '/data/avionics.CSV' }) {
	const [loading, setLoading] = useState(true);
	const [orientation, setOrientation] = useState({
		roll: 0,
		pitch: 0,
		yaw: 0,
		altitude: 0,
	});
	const [isPlaying, setIsPlaying] = useState(false);
	const [frame, setFrame] = useState(0);
	const [showInfo, setShowInfo] = useState(false);
	const [fullscreen, setFullscreen] = useState(false);

	const telemetryRef = useRef<Record<string, string>[]>([]);
	const animationRef = useRef<number | undefined>(undefined);

	useEffect(() => {
		setLoading(true);
		loadDataFromCSV(path)
			.then((data) => {
				telemetryRef.current = data;
				if (data.length > 0) {
					const initialFrameData = data[0];
					setOrientation({
						roll: parseFloat(initialFrameData.roll) || 0,
						pitch: parseFloat(initialFrameData.pitch) || 0,
						yaw: parseFloat(initialFrameData.yaw) || 0,
						altitude: parseFloat(initialFrameData.altitude) || 0,
					});
				}
				setLoading(false);
				setIsPlaying(true);
			})
			.catch((error) => {
				console.error('Error loading data:', error);
				setLoading(false);
			});
	}, [path]);

	useEffect(() => {
		if (animationRef.current) {
			clearInterval(animationRef.current);
			animationRef.current = undefined;
		}

		if (isPlaying && telemetryRef.current.length > 0) {
			animationRef.current = window.setInterval(() => {
				setFrame((prevFrame) => {
					if (prevFrame + 1 >= telemetryRef.current.length) {
						setIsPlaying(false);
						return telemetryRef.current.length;
					}
					const nextFrame = prevFrame + 1;
					const currentFrameData = telemetryRef.current[nextFrame];

					setOrientation({
						roll: parseFloat(currentFrameData.roll) || 0,
						pitch: parseFloat(currentFrameData.pitch) || 0,
						yaw: parseFloat(currentFrameData.yaw) || 0,
						altitude: parseFloat(currentFrameData.altitude) || 0,
					});
					return nextFrame;
				});
			}, 50);
		}

		return () => {
			if (animationRef.current) {
				clearInterval(animationRef.current);
				animationRef.current = undefined;
			}
		};
	}, [isPlaying, frame]);

	const handlePrevFrame = useCallback(() => {
		setFrame((prev) => {
			const newFrame = prev > 0 ? prev - 1 : telemetryRef.current.length - 1;
			const currentFrameData = telemetryRef.current[newFrame];
			setOrientation({
				roll: parseFloat(currentFrameData.roll) || 0,
				pitch: parseFloat(currentFrameData.pitch) || 0,
				yaw: parseFloat(currentFrameData.yaw) || 0,
				altitude: parseFloat(currentFrameData.altitude) || 0,
			});
			return newFrame;
		});
	}, []);

	const handleNextFrame = useCallback(() => {
		setFrame((prev) => {
			const newFrame = (prev + 1) % telemetryRef.current.length;
			const currentFrameData = telemetryRef.current[newFrame];
			setOrientation({
				roll: parseFloat(currentFrameData.roll) || 0,
				pitch: parseFloat(currentFrameData.pitch) || 0,
				yaw: parseFloat(currentFrameData.yaw) || 0,
				altitude: parseFloat(currentFrameData.altitude) || 0,
			});
			return newFrame;
		});
	}, []);

	const handleRestart = useCallback(() => {
		setFrame((prev) => {
			const newFrame = 0;
			const currentFrameData = telemetryRef.current[newFrame];
			setOrientation({
				roll: parseFloat(currentFrameData.roll) || 0,
				pitch: parseFloat(currentFrameData.pitch) || 0,
				yaw: parseFloat(currentFrameData.yaw) || 0,
				altitude: parseFloat(currentFrameData.altitude) || 0,
			});
			return newFrame;
		});
	}, []);

	const togglePlayPause = useCallback(() => {
		setIsPlaying((prev) => !prev);
	}, []);

	return (
		<div className={cn('h-screen relative rounded-2xl overflow-hidden', fullscreen ? 'w-screen' : 'w-[512px]')}>
			{loading && <div className="absolute inset-0 bg-gray-800 flex items-center justify-center text-white z-10">Loading...</div>}
			{showInfo && (
				<div className="absolute top-2 left-3 opacity-80">
					<p>Altitude: {orientation.altitude}</p>
					<p>Roll: {orientation.roll.toFixed(2)}</p>
					<p>Pitch: {orientation.pitch.toFixed(2)}</p>
					<p>Yaw: {orientation.yaw.toFixed(2)}</p>
				</div>
			)}
			<div className="absolute top-2 right-3 z-10 opacity-80">
				<Expand onClick={() => setFullscreen((prev) => !prev)} />
			</div>
			<RocketViewer orientation={orientation} />
			<div className="absolute bottom-6 bg-white/20 left-1/2 -translate-x-1/2 px-4 py-2 flex items-center justify-center gap-4 rounded-4xl">
				<RotateCcw onClick={handleRestart} className="cursor-pointer -mr-1" size={22} />
				<ChevronLeft onClick={handlePrevFrame} className="cursor-pointer" />
				{isPlaying ? <Pause onClick={togglePlayPause} className="cursor-pointer" /> : <Play onClick={togglePlayPause} className="cursor-pointer" />}
				<ChevronRight onClick={handleNextFrame} className="cursor-pointer" />
				<Info onClick={() => setShowInfo((prev) => !prev)} className="cursor-pointer -ml-1" size={22} />
			</div>
		</div>
	);
}
