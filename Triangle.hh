#pragma once

/*
	@author: Xinqi Bao
	Triangle with 3 3-D vectors.
*/

#include <iostream>
#include "Vec3d.hh"
#include "BufferReadBinary.hh"

class Skip {
public:
	friend std::istream& operator >>(std::istream& s, Skip& skip) {
		while (isspace(s.get()) && !s.eof());
		while (!isspace(s.get()) && !s.eof());
		return s;
	}
};
extern Skip skip;

class Triangle {
public:
	Vec3d normal, v1, v2, v3;

	Triangle() {}
	Triangle(std::istream& s){
			s >> skip >> skip >> normal;
			s.ignore(1);s.ignore(256, '\n');
			s >> skip >> v1;
			s >> skip >> v2;
			s >> skip >> v3;
			s.ignore(1);s.ignore(256, '\n');
			s.ignore(256, '\n');
	}
	Triangle(BufferReadBinary& buffer) {
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
};
