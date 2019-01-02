#include "Cross.hh"

bool Cross::equal(const Cross* cr) const {
  return (p1.equal(cr->p1) && p2.equal(cr->p2)) ||
  (p1.equal(cr->p2) && p2.equal(cr->p1));
}
bool Cross::equal(const Cross& cr) const {
  if ((p1.equal(cr.p1) && p2.equal(cr.p2)) || (p1.equal(cr.p2) && p2.equal(cr.p1)))
    return true;
  return false;
}
