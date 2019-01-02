#include "Vec3d.hh"
#include <cmath>
#include "Configurations.hh"

void Vec3d::readBuffer(BufferReadBinary& buffer){
  x = buffer.getFloat(); buffer.stepFloat();
  y = buffer.getFloat(); buffer.stepFloat();
  z = buffer.getFloat(); buffer.stepFloat();
}
bool Vec3d::equal(const Vec3d& v) const {
  if (fabs(x - v.x) < thr && fabs(y - v.y) < thr && fabs(z - v.z) < thr)
  return true;
  return false;
}
bool Vec3d::equal(const Vec3d* v) const {
  return equal(*v);
}
double Vec3d::sum() const {
  return x + y + z;
}
