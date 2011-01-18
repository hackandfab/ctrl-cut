#ifndef RENDERER_H_
#define RENDERER_H_

#include <map>

#include "../util/LaserConfig.h"
#include "../LaserJob.h"
#include "VTypes.h"
#include "Cut.h"

class HPGLEncoder {

public:
  LaserConfig *lconf;
  void encode(Cut *cut, std::ostream &out);
  HPGLEncoder(LaserConfig *lconf);
  virtual ~HPGLEncoder();
};

#endif /* RENDERER_H_ */