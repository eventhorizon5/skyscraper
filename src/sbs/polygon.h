/* $Id$ */

/*
	Scalable Building Simulator - Polygon Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _SBS_POLYGON_H
#define _SBS_POLYGON_H

namespace SBS {

class SBSIMPEXP Polygon
{
public:
	MeshObject* mesh;
	std::vector<TriangleType> triangles;
	Ogre::Plane plane;

	//array holding index extents, to get original geometry
	std::vector<Extents> index_extents;

	//texture mapping matrix and vector
	Ogre::Matrix3 t_matrix;
	Ogre::Vector3 t_vector;

	std::string material; //polygon material
	std::string name; //polygon names

	Polygon(const std::string &name, MeshObject *meshwrapper, std::vector<TriangleType> &triangles, std::vector<Extents> &index_extents, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, const std::string &material, Ogre::Plane &plane);
	~Polygon();
	void GetTextureMapping(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector);
	Ogre::SubMesh* GetSubMesh();
	void GetGeometry(std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly, bool convert = true, bool rescale = true, bool relative = true, bool reverse = false);
	bool PointInside(const Ogre::Vector3 &point, bool plane_check = true, bool convert = true);
	bool IntersectSegment(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float *pr, Ogre::Vector3 &normal, bool convert = true, bool rescale = true);
	bool IntersectRay(std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &start, const Ogre::Vector3 &end);
	bool IntersectSegmentPlane(std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float *pr, Ogre::Vector3 &normal);
	void Move(const Ogre::Vector3 &position, float speed = 1.0f);
	void Delete();
};

}

#endif
