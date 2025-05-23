import { cn } from '@sglara/cn';
import React from 'react';

export default function StateButton({ state, title, currentState, onClick = () => {} }: { state: number; title: string; currentState: number; onClick?: () => void }) {
	return (
		<button className={cn('text-white p-2 m-2 rounded-md', state == currentState ? 'bg-green-500' : 'bg-blue-500')} onClick={onClick}>
			{title}
		</button>
	);
}
