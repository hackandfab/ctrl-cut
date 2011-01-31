#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "stddef.h"
#include "climits"
#include "iostream"
#include <cmath>

#include "VTypes.h"

class Polyline {
public:
  VecEdge edges;
  Polyline();
  virtual ~Polyline(){}

  bool contains(Edge* ls);
  void add(Edge* ls);
  void remove(Edge* ls);
  int count();
  Edge *findLeftmostClockwise();
  VecEdge::iterator find(Edge* ls);

  friend std::ostream& operator <<(std::ostream &os, Polyline &pl);

private:
  static int cnt;
  int id;
};

#endif /* POLYGON_H_ */