/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File   : BottomPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "BottomPanel.h"
#include "ComponentID.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Image.h>
#include <mhgui/Tooltip.h>

using namespace std;
using namespace Animorph;
using namespace mhgui;

#include "ComponentID.h"

BottomPanel::BottomPanel ()
    : Panel (kComponentID_BottomPanel, Rect(0,472,192,32))
{
}

BottomPanel::~BottomPanel ()
{
  delete frame1;
  delete frame2;
  delete frame3;
}

void BottomPanel::createWidgets ()
{
  const float alpha = 0.8;

  frame1 = new Image(kComponentID_Dummy,
                    searchPixmapFile ("ui/frames_01.png"),
                    Rect(0,0,96,16));
  frame1->setAlpha (alpha);
  addWidget (frame1);

  // --------------------------------------------------------------------------
  frame2 = new Image(kComponentID_Dummy,
                    searchPixmapFile ("ui/frames_07.png"),
                    Rect(0,0,32,32));
  frame2->setAlpha (alpha);
  addWidget (frame2);

  // --------------------------------------------------------------------------
  frame3 = new Image(kComponentID_Dummy,
                    searchPixmapFile ("ui/frames_01.png"),
                    Rect(0,0,64,32));
  frame3->setAlpha (alpha);
  addWidget (frame3);
}
