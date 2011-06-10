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

#include <list>
#include "Driver.h"
#include "LaserJob.h"
#include "vector/CutModel.h"
#include "vector/filters/Explode.h"
#include "vector/filters/Join.h"
using std::list;

Driver::Driver() : dumpxml(false)
{
}

Driver::~Driver() {
}

/**
 * run the filter stack. additionally if dumpxml is set to true dump cut graph to xml.
 */
void Driver::filter(LaserJob *job) {
  Explode explode;
  Join join;

   list<CutModel*> cuts = job->getCuts();
   for (list<CutModel*>::iterator it = cuts.begin(); it != cuts.end(); it++) {
     CutModel& cut = *(*it);
     explode.filter(cut);
     join.filter(cut);
   }
}

void Driver::process(LaserJob *job) {
  filter(job);
  std::stringstream ss;
  job->serializeTo(ss);
  LOG_DEBUG_MSG("Output size", ss.str().size());

  std::cout << ss.rdbuf();
}
