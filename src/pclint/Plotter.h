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


#ifndef PLOTTER_H_
#define PLOTTER_H_
#include <map>
#include <iostream>
#include <stdint.h>
#include <cctype>
#include <sstream>
#include <string>
#include <limits>
#include "CImg.h"
#include "2D.h"
#include "PclIntConfig.h"
#include "HPGL.h"
#include "Statistic.h"

using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::numeric_limits;

using namespace cimg_library;

class VectorPlotter {
private:
  BoundingBox *bbox;
  BoundingBox *clip;
  bool down;
  CImg<uint8_t> *imgBuffer;
  uint8_t intensity[4];
public:
  Point penPos;

  VectorPlotter(dim width, dim height, BoundingBox* clip = NULL) :
    bbox(new BoundingBox()), clip(clip), down(false), penPos(0, 0) {
    if (clip != NULL) {
      width = clip->min(width, clip->lr.x - clip->ul.x);
      height = clip->min(height, clip->lr.y - clip->ul.y);
    }
    intensity[0] = 255;
    intensity[1] = 0;
    intensity[2] = 0;
    intensity[3] = 128;

    this->imgBuffer = new CImg<uint8_t> (width, height, 1, 4, 255);
  }

  VectorPlotter(BoundingBox* clip = NULL) :
    bbox(new BoundingBox()), clip(clip), down(false), penPos(0, 0) {
    this->imgBuffer = NULL;
  }

  void penUp() {
    down = false;
    Statistic::singleton()->announcePenUp(SLOT_VECTOR);
  }

  void penDown() {
    down = true;
    Statistic::singleton()->announcePenDown(SLOT_VECTOR);
  }

  void move(coord x, coord y) {
    Point m(x, y);
    move(m);
  }

  void setIntensity(uint8_t intensity) {
    this->intensity[0] = intensity;
  }

  uint8_t getIntensity() {
    return this->intensity[0];
  }

  virtual void draw(const Point& from, const Point& to) {
    if(from == to) {
      Trace::singleton()->warn("zero length drawing operation?");
      return;
    }
    Point drawFrom = from;
    Point drawTo = to;

    coord clip_offX = 0;
    coord clip_offY = 0;

    //apply clipping and update bounding box
    if (this->clip) {
      drawTo = this->clip->shape(drawTo);
      drawFrom = this->clip->shape(drawFrom);
      clip_offX = clip->ul.x;
      clip_offY = clip->ul.y;
    }

    // x coordinates point to the left of a pixel. therefore don't draw the last coordinate
    // This is done before the bbox calculation to avoid an off-by-one error as the bbox
    // is specified in pixels, inclusive the end pixels.
    //drawTo.x--;

    this->bbox->update(drawFrom);
    this->bbox->update(drawTo);

    drawFrom.x -= clip_offX;
    drawFrom.y -= clip_offY;
    drawTo.x -= clip_offX;
    drawTo.y -= clip_offY;

    stringstream ss;
    ss << "\t\t" << drawFrom << " - " << drawTo << " i = " << (unsigned int)this->intensity[0];
    Trace::singleton()->debug(ss.str());

    imgBuffer->draw_line(drawFrom.x, drawFrom.y, drawTo.x, drawTo.y, this->intensity);
  }

  void move(Point& to) {
    if (penPos != to) {
      if (down) {
        draw(penPos, to);
        Statistic::singleton()->announceWork(penPos, to, SLOT_VECTOR);
      } else {
        Statistic::singleton()->announceMove(penPos, to, SLOT_VECTOR);
      }

      this->penPos = to;
      Trace::singleton()->logPlotterStat(penPos);
    }
  }

  virtual BoundingBox* getBoundingBox() {
    return bbox;
  }

  virtual CImg<uint8_t>* getCanvas(CImg<uint8_t>* img = NULL) {
    CImg<uint8_t>* canvas;
    if (PclIntConfig::singleton()->autocrop) {
      canvas = &(imgBuffer->crop(this->bbox->ul.x, this->bbox->ul.y,
          this->bbox->lr.x, this->bbox->lr.y, false));
    } else {
      canvas = imgBuffer;
    }

    if(img != NULL) {
      img->draw_image(*canvas);
      canvas = img;
    }

    return canvas;
  }
};

class BitmapPlotter {
private:
  BoundingBox *bbox;
  BoundingBox *clip;
  uint32_t width;
  uint32_t height;
  uint8_t *imgbuffer;

public:
  Point penPos;

  // width/height is given in bytes
  //
  BitmapPlotter(uint32_t width, uint32_t height, BoundingBox *clip = NULL) :
    bbox(new BoundingBox()), clip(clip), width(width), height(height), penPos(0, 0) {
    if (clip != NULL) {
      this->width = clip->min(width, clip->lr.x - clip->ul.x);
      this->height = clip->min(height, clip->lr.y - clip->ul.y);
    }

    this->imgbuffer = new uint8_t[this->width * this->height];
    memset(this->imgbuffer, 0x00, this->width * this->height);
  }

  BitmapPlotter(BoundingBox* clip = NULL) :
    bbox(new BoundingBox()), clip(clip), penPos(0, 0) {
    this->imgbuffer = NULL;
  }

  void move(coord x, coord y) {
    Point p(x, y);
    move(p);
  }

  void move(Point &to) {
    Statistic::singleton()->announceMove(penPos, to, SLOT_RASTER);
    this->penPos = to;
    // Trace::singleton()->logPlotterStat(penPos);
  }

  void writeBitmap(uint8_t bitmap) {
    this->fill(bitmap, 1);
  }

  void fill(uint8_t bitmap, int len) {
    int dir = (len < 0) ? -1 : 1;
    Point pos = this->penPos;
    Point to = this->penPos;
    to.x += (len * 8);

    Statistic::singleton()->announcePenDown(SLOT_RASTER);
    Statistic::singleton()->announceWork(penPos, to, SLOT_RASTER);

    if (this->clip) {
      if (this->penPos.y < this->clip->ul.y || this->penPos.y > this->clip->lr.y) return;
      pos.x -= this->clip->ul.x;
      pos.y -= this->clip->ul.y;
    }
    for (int i=0;i<abs(len);i++) {
      if (this->clip) {
        if ((this->penPos.x + i) < this->clip->ul.x || (this->penPos.x + i) > this->clip->lr.x) return;
      }
      this->imgbuffer[pos.y * this->width + pos.x + i*dir] = bitmap;
      this->bbox->update(this->penPos.x + i*dir, this->penPos.y);
    }
    Statistic::singleton()->announcePenUp(SLOT_RASTER);
  }

  virtual BoundingBox* getBoundingBox() {
    return bbox;
  }

  virtual CImg<uint8_t>* getCanvas(CImg<uint8_t>* img = NULL) {
    if(!this->bbox->isValid())
      return NULL;

    Point start(0,0);
    Point size(this->width, this->height);
    if (PclIntConfig::singleton()->autocrop) {
      start.x = this->bbox->ul.x;
      start.y = this->bbox->ul.y;

      size.x = this->bbox->lr.x - this->bbox->ul.x + 1;
      size.y = this->bbox->lr.y - this->bbox->ul.y + 1;
    }

    CImg<uint8_t>* canvas;

    if(img != NULL)
      canvas = img;
    else
      canvas = new CImg<uint8_t>(size.x*8, size.y, 1, 1, 255);

    for (uint32_t y=0;y<size.y;y++) {
      for (uint32_t x=0;x<size.x;x++) {
        uint8_t bitmap = this->imgbuffer[(y + start.y)*this->width + (x + start.x)];
        for (int b=0;b<8;b++) {
          uint8_t val = (bitmap & (0x80 >> b)) ? 0 : 255;
          if(val == 0)
            canvas->draw_point(x*8 + b, y, &val);
        }
      }
    }

    return canvas;
  }
};

#endif /* PLOTTER_H_ */
