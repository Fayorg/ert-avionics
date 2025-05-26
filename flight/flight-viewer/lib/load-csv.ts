export async function loadDataFromCSV(path: string = '/data/avionics.CSV') {
	const res = await fetch(path);
	if (!res.ok) {
		throw new Error('Network response was not ok');
	}

	const raw = await res.text();
	const lines = raw
		.split('\n')
		.map((line) => line.trim())
		.filter((line) => line.length > 0);
	const headers = lines[0].split(',');
	const data = lines.slice(1).map((line) => {
		const values = line.split(',');
		const entry: Record<string, string> = {};
		headers.forEach((header, index) => {
			entry[header] = values[index];
		});
		return entry;
	});
	return data;
}