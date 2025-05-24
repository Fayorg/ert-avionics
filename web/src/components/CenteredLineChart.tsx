import React from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, ReferenceLine } from 'recharts';

const CenteredLineChart = ({ data, title = 'Test data' }: { data: number[]; title: string }) => {
	const formattedData = data.map((item, index) => ({
		value: item,
		index: index, // You can also use index for X-axis if you want to show it
	}));

	const maxYDataValue = formattedData.length > 0 ? Math.max(...formattedData.map((item) => Math.abs(item.value))) : 1;
	const yDomainSymmetrical = [-2 * Math.PI, 2 * Math.PI];

	return (
		<div className="w-full h-full border-1 border-white/80 rounded-lg overflow-hidden relative">
			<h2 className="absolute left-1 top-1">{title}</h2>
			<ResponsiveContainer width="100%" height="100%">
				{/* Pass the formattedData to LineChart */}
				<LineChart data={formattedData} margin={{ top: 0, right: 0, left: 0, bottom: 0 }}>
					<YAxis domain={yDomainSymmetrical} tick={false} hide />
					{/* You can still hide XAxis if you don't need explicit x-axis values */}
					<XAxis hide />
					<ReferenceLine y={0} stroke="#FF0000" strokeDasharray="3 3" />
					{/* Crucial: Specify dataKey for the Line component */}
					<Line type="monotone" dataKey="value" stroke="#8884d8" />
				</LineChart>
			</ResponsiveContainer>
		</div>
	);
};

export default CenteredLineChart;
