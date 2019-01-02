#include "Command.hh"
using namespace std;

double Command::e = 0;

ostream& operator <<(ostream& s, const Command& c) {
	switch (c.cmd) {
	case 0:
		return s << "G0 F6600 X" << c.x << " Y" << c.y << '\n';
	case 1:
		Command::e += c.de;
		return s << "G1 F783 X" << c.x << " Y" << c.y << " E" << Command::e << '\n';
	}
}

void Command::G0_high(ostream& s, double z) {
	s << "G0 F6600  Z" << z << '\n';
}
void Command::resetE(std::ostream& s) {
	e = 0;
	s << "G92 E0 ; reset the expected extruder position\n";
}

