#ifndef RENDERER_H_
#define RENDERER_H_

#include <map>
#include "LaserJob.h"
#include "config/LaserConfig.h"
#include "vector/model/CutModel.h"

class HPGLEncoder {

public:
  LaserConfig *lconf;
  void encode(CutModel& model, std::ostream &out) const;
  HPGLEncoder(LaserConfig *lconf);
  virtual ~HPGLEncoder();
};

#endif /* RENDERER_H_ */
