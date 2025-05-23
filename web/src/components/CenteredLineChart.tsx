import React from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, ReferenceLine } from 'recharts';

const CenteredLineChart = ({ data, title = 'Test data' }: { data: number[]; title: string }) => {
	const maxYDataValue = data.length > 0 ? Math.max(...data.map((item) => Math.abs(item))) : 1;
	const yDomainSymmetrical = [-maxYDataValue * 1.1, maxYDataValue * 1.1];

	return (
		<div className="w-full h-full border-1 border-white/80 rounded-lg overflow-hidden relative">
			<h2 className="absolute left-1 top-1">{title}</h2>
			<ResponsiveContainer width="100%" height="100%">
				<LineChart data={data} margin={{ top: 0, right: 0, left: 0, bottom: 0 }}>
					<YAxis domain={yDomainSymmetrical} tick={false} hide />
					<XAxis hide />
					<ReferenceLine y={0} stroke="#FF0000" strokeDasharray="3 3" />
					<Line type="monotone" dataKey="y" stroke="#8884d8" />
				</LineChart>
			</ResponsiveContainer>
		</div>
	);
};

export default CenteredLineChart;
