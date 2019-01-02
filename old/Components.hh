#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "Configurations.hh"
//#include "HashMapVec.hh"
#include "BufferReadBinary.hh"
#include "BufferWrite.hh"

class Skip {
public:
	friend std::istream& operator >>(std::istream& s, Skip& skip) {
		while (isspace(s.get()) && !s.eof());
		while (!isspace(s.get()) && !s.eof());
		return s;
	}
};
extern Skip skip;
extern char c[256];

class Vec3d {
public:
	double x, y, z;

	Vec3d() {}
	Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

	void readBuffer(BufferReadBinary& buffer){
		x = buffer.getFloat(); buffer.stepFloat();
		y = buffer.getFloat(); buffer.stepFloat();
		z = buffer.getFloat(); buffer.stepFloat();
	}
	friend std::istream& operator >>(std::istream& s, Vec3d& v) {
		return s >> v.x >> v.y >> v.z;
	}
	friend std::istream& operator >>(std::istream& s, Vec3d* v) {
		return s >> v->x >> v->y >> v->z;
	}
	friend std::ostream& operator <<(std::ostream& s, const Vec3d& v) {
		return s << v.x << "," << v.y << "," << v.z;
	}
	friend std::ostream& operator <<(std::ostream& s, const Vec3d* v) {
		return s << v->x << "," << v->y << "," << v->z;
	}
	bool equal(const Vec3d& v) const {
		if (fabs(x - v.x) < thr && fabs(y - v.y) < thr && fabs(z - v.z) < thr)
			return true;
		return false;
	}
	bool equal(const Vec3d* v) const { return equal(*v); }
	double sum() const { return x + y + z; }
};

class Vec2d {
public:
	double x, y;

	Vec2d() {}
	Vec2d(double x, double y) : x(x), y(y) {}
	Vec2d( const Vec3d& v1, const Vec3d& v2, double z) {
		x = (v1.x - v2.x) * (z - v1.z) / (v1.z - v2.z) + v1.x;
		y = (v1.y - v2.y) * (z - v1.z) / (v1.z - v2.z) + v1.y;
	}

	friend std::istream& operator >>(std::istream& s, Vec2d& v) {
		return s >> v.x >> v.y;
	}
	friend std::ostream& operator <<(std::ostream& s, const Vec2d& v) {
		return s << v.x << "," << v.y;
	}
	friend std::ostream& operator <<(std::ostream& s, const Vec2d* v) {
		return s << v->x << "," << v->y;
	}
	bool equal(const Vec2d& v) const {
		if (fabs(x - v.x) < thr && fabs(y - v.y) < thr)
			return true;
		return false;
	}
	bool equal(const Vec2d* v) const { return equal(*v); }
	double sum() const { return x + y; }
};

class Triangle {
public:
	Vec3d normal, v1, v2, v3;

	Triangle() {}
	Triangle(std::istream& s){
			//s >> c >> normal >> c >> c >> c >> v1 >> c >> v2 >> c >> v3;
			/*
			s.ignore(14) >> normal;
			s.ignore(28) >> v1;
			s.ignore(13) >> v2;
			s.ignore(13) >> v3;
			s.ignore(24);
			*/
			s >> skip >> skip >> normal;
			s.ignore(1);s.ignore(256, '\n');
			s >> skip >> v1;
			s >> skip >> v2;
			s >> skip >> v3;
			s.ignore(1);s.ignore(256, '\n');
			s.ignore(256, '\n');
	}
	Triangle(BufferReadBinary& buffer) {
		/*normal = buffer.read();
		v1 = buffer.read();
		v2 = buffer.read();
		v3 = buffer.read();
		buffer.endTriangle();*/
		normal.readBuffer(buffer);
		v1.readBuffer(buffer);
		v2.readBuffer(buffer);
		v3.readBuffer(buffer);
		buffer.step2Bytes();
	}
	Triangle(Vec3d& normal, Vec3d& v1, Vec3d& v2, Vec3d& v3) : normal(normal), v1(v1), v2(v2), v3(v3) {}
	friend std::ostream& operator <<(std::ostream& s, Triangle& tri) {
		return s <<
			"normal: " << tri.normal << '\n' <<
			"vertex: " << tri.v1 << '\n' <<
			"vertex: " << tri.v2 << '\n' <<
			"vertex: " << tri.v3 << '\n';
	}
	friend std::istream& operator >>(std::istream& s, Triangle& t) {
		return s >> c >> t.normal >> c >> c >> c >> t.v1 >> c >> t.v2 >> c >> t.v3;
	}
};

class Cross {
public:
	Vec2d p1, p2;

	Cross(Vec2d* p1, Vec2d* p2) :p1(*p1), p2(*p2) {}
	Cross(const Vec2d& p1, const Vec2d& p2) : p1(p1), p2(p2) {}
	friend std::ostream& operator <<(std::ostream& s, const Cross* cr) {
		return s << cr->p1 << "  ---  " << cr->p2;
	}
	friend std::ostream& operator <<(std::ostream& s, const Cross& cr) {
		return s << cr.p1 << "  ---  " << cr.p2;
	}
	bool equal(const Cross* cr) const {
		return (p1.equal(cr->p1) && p2.equal(cr->p2)) ||
			(p1.equal(cr->p2) && p2.equal(cr->p1));
	}
	bool equal(const Cross& cr) const {
		if ((p1.equal(cr.p1) && p2.equal(cr.p2)) || (p1.equal(cr.p2) && p2.equal(cr.p1)))
			return true;
		return false;
	}
};

class Loop {
private:
	bool completed;
	double minX, maxX, minY, maxY;
	double xsum = 0, ysum = 0;
	std::vector<Vec2d> head;
public:
	Loop(Vec2d& p, uint32_t size = 10) : completed(false) {
		head.reserve(size);
		head.push_back(p);
		minX = p.x, maxX = p.x;
		minY = p.y, maxY = p.y;
	}
	bool isCompleted() const { return completed; }
	void add(double x, double y) { add(new Vec2d(x, y)); }
	void add(Vec2d* p) { add(*p); }
	void add(Vec2d& p) {
		if (head.front().equal(p)) {	//Loop complete
			completed = true;
			return;
		}
		if (minX > p.x)
			minX = p.x;
		if (maxX < p.x)
			maxX = p.x;
		if (minY > p.y)
			minY = p.y;
		if (maxY < p.y)
			maxY = p.y;
		head.push_back(p);
	}
	int size() const { return head.size(); }
	const Vec2d& at(int i) const{ return head.at(i % head.size()); }
	const Vec2d& front() const { return head.front(); }
	const Vec2d& back() const { return head.back(); }
	Vec2d& at(int i) { return head.at(i % head.size()); }
	Vec2d& front() { return head.front(); }
	Vec2d& back() { return head.back(); }
	double getminX() const { return minX; }
	double getmaxX() const { return maxX; }
	double getminY() const { return minY; }
	double getmaxY() const { return maxY; }
	double getXsum() const { return xsum; }
	double getYsum() const { return ysum; }
	void loopSum() {
		for (const auto& v : head) {
			xsum += v.x;
			ysum += v.y;
		}
	}
	void optimize() {
		if (head.size() < 4)
			return;
		Vec2d p1(head.back());	//position at -1, 0, 1
		Vec2d p2(head.front());
		Vec2d p3(head.at(1));
		if (fabs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) < thr)
			head.erase(head.begin());

		for (int i = 0; i < head.size() - 2; i++) {	//position form 0, 1, 2   to   end-2, end-1, end
			p1 = head.at(i);
			p2 = head.at(i + 1);
			p3 = head.at(i + 2);
			if (fabs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) < thr) {	//in same line
				head.erase(head.begin() + (i + 1));
				i--;
			}
		}

		p1 = head.at(head.size() - 2);	//position at -2, -1, 0
		p2 = head.back();
		p3 = head.front();
		if (fabs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) < thr)
			head.pop_back();
	}
	void print() const {
		if (head.size() == 0) {
			std::cout << "null\n";
			return;
		}
		std::cout << head.at(0);
		for (int i = 1; i < head.size(); i++) {
			std::cout << " --> " << head.at(i);
		}
	}
};

class Command {
public:
	uint16_t cmd;
	double x, y, de = 0;
	Command(uint16_t cmd, double x, double y) : cmd(cmd), x(x), y(y) {}
	/*************************************************************************/
	/*
		part for to file directly
	*/
	friend std::ostream& operator <<(std::ostream& s, const Command& c) {
		switch (c.cmd) {	//TODO: figure out the more efficient way, without switch!!
		case 0:
			return s << "G0 F6600 X" << c.x << " Y" << c.y << '\n';
		case 1:
			e += c.de;
			return s << "G1 F783 X" << c.x << " Y" << c.y << " E" << e << '\n';
		}
	}
	static double e;
	static void G0_high(std::ostream& s, double z);
	static void resetE(std::ostream& s);
	/*************************************************************************/
	/*************************************************************************/
	/*
		part for to writing buffer
	*/
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
	static void G0_high(BufferWrite& buf, double z);
	static void resetE(BufferWrite& buf);
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
	std::vector<Loop> body_loops;
	std::vector<Command> body_commands;
	std::vector<Loop> support_loops;
	std::vector<Command> support_commands;
	std::vector<std::vector<Cross>> parts;
public:
	Layer(double z) :z(z) {}
	double getZ() const { return z; }
	double& getMinX() { return minX; }
	double& getMaxX() { return maxX; }
	std::vector<Loop>& getBodyLoops() { return body_loops; }
	std::vector<Command>& getBodyCommands() { return body_commands; }
	std::vector<std::vector<Cross>>& getParts() { return parts; }
	void commandsOut(std::ostream& s) const {	//output to gcode file
		Command::G0_high(s, z);
		Command::resetE(s);
		for (const auto& c : body_commands)
			s << c;
		s.flush();
	}
	void commandsOut(BufferWrite& buf) const {	//output to writing buffer
		Command::G0_high(buf, z);
		Command::resetE(buf);
		for (const auto& c : body_commands)
			buf << c;
		//buf.flush();
	}
	/*
		Calculate how much to extrude
	*/
	void generateDe() {
		tx = body_commands.front().x;
		ty = body_commands.front().y;
		for (auto& tc : body_commands) {
			if (tc.cmd == 1) {
				tc.de = sqrt(pow(tc.x - tx, 2) + pow(tc.y - ty, 2)) * ef;
			}
			tx = tc.x;
			ty = tc.y;
		}
	}
};
