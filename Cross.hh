#pragma once
#include <iostream>
#include "Vec2d.hh"

class Cross {
public:
  Vec2d p1, p2;
  bool equal(const Cross* cr) const;
  bool equal(const Cross& cr) const;

  Cross(Vec2d* p1, Vec2d* p2) :p1(*p1), p2(*p2) {}
  Cross(const Vec2d& p1, const Vec2d& p2) : p1(p1), p2(p2) {}

  friend std::ostream& operator <<(std::ostream& s, const Cross* cr) {
    return s << cr->p1 << "  ---  " << cr->p2;
  }
  friend std::ostream& operator <<(std::ostream& s, const Cross& cr) {
    return s << cr.p1 << "  ---  " << cr.p2;
  }
};
