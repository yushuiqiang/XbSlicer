#pragma once

/*
	@author: Xinqi Bao
	Main class to manage the whole program.
	Include reading file, slicing each layer and writting to file,
	using mult-threads.
*/

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include "Configurations.hh"
#include "STL.hh"
#include "Layer.hh"

class Slicer {
private:
	Stl stl;
	int numLayer;
	int finishedCount = 0;	//count for each finished layers
	bool* checkLayers;
	std::string stl_target;	//original stl file name
	std::string gcode_target;	//the name for outputting gcode file
	std::vector<Layer> layers;	//contain each layers with loops and gcode
	std::thread threads[NUM_THREADS];
	std::mutex checkMutex;
	std::mutex countMutex;
	void threadLayer();
public:
	void slicing();
	Slicer(std::string& filename) :Slicer(filename.c_str()) {}
	Slicer(const char* filename) {
	#if 0	//Make directory for Windows
		_mkdir(gcode_filePath.c_str());	//make directory for storing gcode
	#endif
	#if 0
		std::string mkdir = "mkdir -p ";
		int res = system(mkdir.append(gcode_filePath.c_str()).c_str());
	#endif

		//TODO:	STL file extensions: .stl, .stlb, .bif.
		//This can work for .stl and .stlb, but not good.
		int extensionLen = 3;
		bool isBinary;
		if(filename[strlen(filename) - 1] == 'b'){
			isBinary = true;
			extensionLen++;
		}
		else
			isBinary = false;

		//change the file extension from ".stl" to ".gcode"
		stl_target = stl_filePath; stl_target.append(filename);
		gcode_target = gcode_filePath;
		gcode_target.append(filename, strlen(filename) - extensionLen); gcode_target.append("gcode");
		remove(gcode_target.c_str());	//remove old gcode file
		std::cout << "\tFile: " << filename << "\nReading...";
		std::cout << "\nNumber of triangles: " << stl.read(stl_target, isBinary);
		std::cout << "\nRead finish\n";
	}
};
