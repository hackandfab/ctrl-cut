/*
 * EpilogCUPS - A laser cutter CUPS driver
 * Copyright (C) 2009-2010 Amir Hassan <amir@viel-zu.org> and Marius Kintel <marius@kintel.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef RASTER_H_
#define RASTER_H_

#include <stdlib.h>
#include <stdint.h>
#include "CImg.h"
#include "Pcl.h"
#include "2D.h"
#include "Plot.h"

class Run {
private:
  PclInstr* dataInstr;

public:
  Point loc;

  uint8_t length;
  coord lineLen;
  coord linePos;

  bool fill;
  bool reverse;

  Run(): length(0), lineLen(0), linePos(0) {}

  Run* init(PclInstr* yInstr, PclInstr* xInstr, PclInstr* pixelLen,  PclInstr* dataInstr) {
    this->loc.y = yInstr->value;
    this->loc.x = xInstr->value;
    this->lineLen = abs(pixelLen->value);
    this->linePos = 0;
    this->dataInstr = dataInstr;
    this->reverse = pixelLen->value < 0;
    return this->nextRun();
  }

  Run* nextRun() {
    if(dataInstr->limit < 2) {
      Trace::singleton()->printBacklog(cerr, "short read: run length");
      exit(2);
    } else {
      uint8_t rl = dataInstr->next();
      if(rl > 128) {
        this->fill = true;
        this->length  = 257 - rl;
      } else if(rl < 128) {
        this->fill = false;
        this->length  = rl + 1;
      } else {
        this->length = 0;
      }
      return this;
    }
  }

  uint8_t nextIntensity() {
      if (!this->dataInstr->hasNext()) {
        Trace::singleton()->printBacklog(cerr, "short read: intensity");
        exit(2);
      } else
      return 255 - this->dataInstr->next();
  }
};

class RasterPlotter {
private:
  PclPlotter* plotter;

public:
  Run* currentRun;

  RasterPlotter(PclPlotter* plotter) :
    plotter(plotter), currentRun(NULL) {
  }

  bool decode(Run *run) {
    this->currentRun = run;

    if (run == NULL || run->length == 0) {
      cerr << "invalid run" << endl;
      return NULL;
    }

    Point start;
    Point end;

    if(run->reverse) {
      start = end = run->loc;
      start.x = run->loc.x + ((run->lineLen - 1) * 8) - (run->linePos * 8);
      end.x = start.x - (run->length * 8);
    } else {
      start = end = run->loc;
      start.x = start.x + (run->linePos * 8);
      end.x = start.x + (run->length * 8);
    }

    string dirstring = run->reverse ? " <- " : " -> ";
    //cerr << "\t" << start << dirstring << end << endl;

    this->plotter->penUp();
    this->plotter->move(start);
   //if (!run->reverse)
    this->plotter->penDown();

    if(run->fill) {
      plotter->setIntensity(run->nextIntensity());
      plotter->move(end);
    } else {
      int8_t dir = run->reverse ? -8 : 8;

      for (int i = 0; i < run->length; ++i) {
        plotter->setIntensity(run->nextIntensity());
        plotter->move(start.x + (i * dir), start.y);
      }
    }

    run->linePos+=run->length;
    if(run->linePos < run->lineLen)
      return true;
    else
      return false;
  }
};
#endif /* RASTER_H_ */