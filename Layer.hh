#pragma once
#include <iostream>
#include <vector>
#include "BufferWrite.hh"
#include "Loop.hh"
#include "Cross.hh"

class Command {
public:
	uint16_t cmd;
	double x, y, de = 0;
	Command(uint16_t cmd, double x, double y) : cmd(cmd), x(x), y(y) {}
	/*************************************************************************/
	/*
		part for to file directly
	*/
  static double e;
  static void G0_high(std::ostream& s, double z);
  static void resetE(std::ostream& s);
	friend std::ostream& operator <<(std::ostream& s, const Command& c) {
		switch (c.cmd) {	//TODO: figure out the more efficient way, without switch!!
		case 0:
			return s << "G0 F6600 X" << c.x << " Y" << c.y << '\n';
		case 1:
			e += c.de;
			return s << "G1 F783 X" << c.x << " Y" << c.y << " E" << e << '\n';
		}
	}
	/*************************************************************************/
	/*************************************************************************/
	/*
		part for to writing buffer
	*/
  static void G0_high(BufferWrite& buf, double z);
  static void resetE(BufferWrite& buf);
	friend BufferWrite& operator <<(BufferWrite& buf, const Command& c) {
		switch (c.cmd) {	//TODO: figure out the more efficient way, without switch!!
		case 0:
			buf.writeG0(c.x, c.y);
			return buf;
		case 1:
			e += c.de;
			buf.writeG1(c.x, c.y, e);
			return buf;
		}
	}
	/*************************************************************************/
};

/*
	Each sliced layer
*/
class Layer {
private:
	double z;
	double tx, ty;
	double minX, maxX;
	std::vector<Loop> loops;
	std::vector<Command> commands;
	std::vector<std::vector<Cross>> parts;
public:
	double getZ() const;
	double& getMinX();
	double& getMaxX();
	std::vector<Loop>& getLoops();
	std::vector<Command>& getCommands();
	std::vector<std::vector<Cross>>& getParts();
	void commandsOut(std::ostream& s) const; //output to gcode file
	void commandsOut(BufferWrite& buf) const; //output to writing buffer
	void generateDe(); //Calculate how much to extrude
	Layer(double z) :z(z) {}
};
