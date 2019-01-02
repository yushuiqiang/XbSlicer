#pragma once

#include <iostream>

class RouterCommand {
public:
	double x, y;
	RouterCommand(double x, double y) : x(x), y(y) {}
	virtual void doit(std::ostream& s) = 0;
	friend std::ostream& operator <<(std::ostream& s, const RouterCommand& c);
	
};

class GotoZ : 
class GotoXY : public RouterCommand {
public:
	GotoXY(double x, double y) : RouterCommand(x,y) {}
	virtual void doit(std::ostream& s) { s << "G0 X" << x << " Y" << y << '\n'; }
};

class RelativeXY : public RouterCommand {
public:
	RelativeXY(double x, double y) : RouterCommand(x,y) {}
	virtual void doit(std::ostream& s) { s << "G1 X" << x << " Y" << y << '\n'; }
	
};
