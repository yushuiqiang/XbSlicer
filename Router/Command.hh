#pragma once

#include <iostream>

class Command {
public:
	uint16_t cmd;
	double x, y, de = 0;
	Command(uint16_t cmd, double x, double y) : cmd(cmd), x(x), y(y) {}
	friend std::ostream& operator <<(std::ostream& s, const Command& c);

	static double e;
	static void G0_high(std::ostream& s, double z);
	static void resetE(std::ostream& s);
};
