#include "RouterGCode.hh"

int main() {
	RouterGCode g("test.gcode", "pre.gcode", "post.gcode");
	g.pre();
	g.gotoXY(0,0);
	g.gotoXY(1.5,0);
	g.gotoXY(0,2.5);
	g.gotoZ(1.0);
	g.post();
	g.write();
}
