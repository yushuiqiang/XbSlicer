#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Configuration.hh"
//#include "Loop.hh"
//#include "Layer.hh"
//#include "Cross.hh"

class RouterGCode {
private:
	const std::string filename;
	double zMin; // DON'T go below this one!!!
	double zMax; // the top of the block of material to be carved;
	double dz; // the layer depth to be carved at one time (not too deep!)
	
	int numParts;
	double xMin, xMax, yMin, yMax;
	//	std::vector<Loop>& loops;
	//	std::vector<RouterCommand>& commands;
	std::string gcode;
	//	std::vector<std::vector<Cross>>& parts;
	void append(const char cmd[], double v1, const char sep[], double v2) {
		std::string temp = cmd + std::to_string(v1) + sep + std::to_string(v2) + '\n';
		gcode += temp;
		//		if (verbose)
		std::cout << temp;
	}
	template<typename T>
	void append(const char cmd[], T v1) {
		std::string temp = cmd + std::to_string(v1) + '\n';
		gcode += temp;
		//		if (verbose)
		std::cout << temp;
	}

public:
	RouterGCode(const std::string& filename,
							const std::string& pre,
							const std::string& post) : filename(filename),
																				 xMin(0), xMax(0), yMin(0), yMax(0) {
	}
	void pre() {
		gcode = R"foo(
%
G17 G20 G90 G94
G0 Z0.1250 ; lift spindle
X-0.0 Y0.
)foo";
	}
	void post() {
		gcode += "%\n";
	}
	void gotoXY(double x, double y) {
		append("G0 X", x, " Y", y);
	}
	void relativeXY(double dx, double dy) {
		append("G1 X", dx, " Y", dy);
	}

	void gotoZ(double z) {
		append("G0 Z", z);
	}

	void relativeZ(double dz) {
		append("G1 Z", dz);
	}

	void waitMillisl(int pauseMillis) {
		append("G4 P", pauseMillis);
	}
  void waitForToolchange() {
		char status = ' ';
		do {
			std::cin >> status;
		} while (status != 'C' && status != 'c'); // type c to go on
	}
	void write() {
		std::cout << gcode;
		std::ofstream f(filename);
		f << gcode;
	}
	void outline();
	void fill();
};
