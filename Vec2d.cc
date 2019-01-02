#include "Vec2d.hh"
#include <cmath>
#include "Configurations.hh"

bool Vec2d::equal(const Vec2d& v) const {
  if (fabs(x - v.x) < thr && fabs(y - v.y) < thr)
    return true;
  return false;
}
bool Vec2d::equal(const Vec2d* v) const {
  return equal(*v);
}
double Vec2d::sum() const {
  return x + y;
}
