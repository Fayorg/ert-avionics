import { cn } from '@sglara/cn';
import React, { useEffect } from 'react';

export function Heartbeat({ heartbeat }: { heartbeat: { timestamp: number; status: number } }) {
	const [color, setColor] = React.useState('bg-gray-500');
	const [text, setText] = React.useState('DISCONNECTED');
	const [lastHeartbeat, setLastHeartbeat] = React.useState('-');
	const [state, setState] = React.useState(getStateText());

	useEffect(() => {
		updateStates();
		setState(getStateText());

		let task = setInterval(() => {
			updateStates();
		}, 1000);

		return () => {
			clearInterval(task);
		};
	}, [heartbeat]);

	function updateStates() {
		if (heartbeat.timestamp == 0) {
			setText('DISCONNECTED');
			setLastHeartbeat('-');
			return setColor('bg-gray-500');
		} else if (new Date().getTime() - new Date(heartbeat.timestamp).getTime() <= 10 * 1000) {
			setText('CONNECTED');
			setLastHeartbeat(((new Date().getTime() - new Date(heartbeat.timestamp).getTime()) / 1000).toFixed(0) + 's');
			return setColor('bg-green-500');
		} else if (new Date().getTime() - new Date(heartbeat.timestamp).getTime() <= 15 * 1000) {
			setText('CONNECTED');
			setLastHeartbeat(((new Date().getTime() - new Date(heartbeat.timestamp).getTime()) / 1000).toFixed(0) + 's');
			return setColor('bg-orange-500');
		} else {
			setText('INTERRUPTED');
			setColor('bg-red-500');
			setLastHeartbeat(((new Date().getTime() - new Date(heartbeat.timestamp).getTime()) / 1000).toFixed(0) + 's');
		}
	}

	function getStateText() {
		switch (heartbeat.status) {
			case 0:
				return 'INIT';
			case 1:
				return 'READY';
			case 2:
				return 'ARMED';
			case 3:
				return 'IN-FLIGHT';
			case 4:
				return 'GLIDING';
			case 5:
				return 'LANDED';
			default:
				return 'UNKNOWN';
		}
	}

	return (
		<div className="absolute top-0 left-0 border-b-1 border-r-1 rounded-br-2xl p-2 flex flex-col">
			<div className="flex flex-row items-center gap-2">
				<div className={cn('w-3 h-3 rounded-full', color)} />
				<p>{text}</p>
			</div>
			<div>
				<p>Last beat: {lastHeartbeat}</p>
				<p>State: {getStateText()}</p>
			</div>
		</div>
	);
}
