/*
 * Ctrl-Cut - A laser cutter CUPS driver
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

#ifndef DITHERSIERRA_H_
#define DITHERSIERRA_H_

#include "Dither.h"

class DitherSierra3: public Dither {
public:
  DitherSierra3(GrayscaleImage& img): Dither(img) {}
  virtual ~DitherSierra3(){}
  void ditherPixel(const uint32_t x, const uint32_t y, Pixel<uint8_t>& pix, const uint32_t colors = 1);

};

#endif /* DITHERSIERRA_H_ */
