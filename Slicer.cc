#include "Slicer.hh"
#include <fstream>
#include "Slicing.hh"
#include "BufferWrite.hh"
using namespace std;

void Slicer::slicing() {
	double zmax = stl.getZmax();
	double zmin = stl.getZmin();
	numLayer = (zmax - zmin - zStart) / zgap + 1;
	layers.reserve(numLayer);
	checkLayers = new bool[numLayer];
	for (int i = 0; i < numLayer; i++)
		checkLayers[i] = false;
	for (double z = zmin + zStart; z < zmax; z += zgap) {
		layers.push_back(Layer(z));
	}
	cout << finishedCount << "/" << numLayer;
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i] = thread(&Slicer::threadLayer, this);
	}
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i].join();
	}
	/*
		Support material part, not required for now,
		it's also a big project
	*/
	/*
	if (needSupportMaterial) {
		cout << "\nAdding Support Material...";
		//addSupportMaterial();
	}
	*/
	cout << "\nWriting...";
	#if 0	//Write to gcode file directly
	ofstream f(gcode_target, ios_base::out | ios_base::app);
	for (const auto& layer : layers)
		layer.commandsOut(f);
	#endif
	#if 1	//Write to writing buffer
	BufferWrite buffer(gcode_target);
	for (const auto& layer : layers)
		layer.commandsOut(buffer);
	buffer.flush();
	#endif
	cout << "\nDone!\n";
}

//mult-threads management function
void Slicer::threadLayer() {
	for (int index = 0; index < numLayer; index++) {
		checkMutex.lock();
		if (checkLayers[index]) {
			checkMutex.unlock();
			continue;
		}
		checkLayers[index] = true;
		checkMutex.unlock();

		Slicing cl(stl, layers[index]);

		countMutex.lock();
		finishedCount++;
		cout << "\r" << finishedCount << "/" << numLayer;
		countMutex.unlock();
	}
}
