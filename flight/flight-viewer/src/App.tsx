import './App.css';
import RocketPlayer from '../components/3d-rocket-player';

function App() {
	return (
		<div className="h-screen w-screen flex items-center justify-between bg-gray-900 text-white">
			<RocketPlayer />
			{/* <RocketPlayer path="/data/avionics.CSV" /> */}
		</div>
	);
}

export default App;
