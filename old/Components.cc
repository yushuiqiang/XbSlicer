#include "Components.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "Configurations.hh"
//#include "HashMapVec.hh"
#include "BufferReadBinary.hh"
#include "BufferWrite.hh"

double Command::e = 0;
void Command::G0_high(std::ostream& s, double z) {
	s << "G0 F6600  Z" << z << '\n';
}
void Command::resetE(std::ostream& s) {
	e = 0;
	s << "G92 E0 ; reset the expected extruder position\n";
}

void Command::G0_high(BufferWrite& buf, double z) {
	buf << "G0 F6600  Z" << z << '\n';
}
void Command::resetE(BufferWrite& buf) {
	e = 0;
	buf << "G92 E0 ; reset the expected extruder position\n";
}
