/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005  Andreas Volz
 *  Copyright (C) 2006-2007  MakeHuman Project
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
 *  File: ObjExporter.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef OBJEXPORTER_H
#define OBJEXPORTER_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Matrix.h"
#include "Mesh.h"
#include "util.h"
#include <fstream>
#include <iostream>

using std::ostringstream;
using std::string;

namespace Animorph
{

/*! \brief Exports a Mesh in Wavefront OBJ format to an ostringstream
 *
 * See http://en.wikipedia.org/wiki/Obj
 */
class ObjExporter
{
protected:
	Mesh & mesh;
	Matrix tm;

public:
	/*!
	 * \param _mesh construct ObjExporter from a Mesh object
	 */
	ObjExporter(Animorph::Mesh & _mesh)
	        : mesh(_mesh)
	{
		tm.identity();
	}

	/*!
	 * \param tm the Matrix which transforms the Mesh before exporting
	 */
	void setTransformationMatrix(Matrix & tm) { this->tm = tm; }

	/// Export the Mesh in OBJ file and MTL file with same name, but ending .mtl

	bool exportFile(const string & exportpath);
};

} // namespace Animorph

#endif // OBJEXPORTER_H
