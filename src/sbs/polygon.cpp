/* $Id$ */

/*
	Scalable Building Simulator - Polygon Class
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

#include "globals.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

void Polygon::GetTextureMapping(Ogre::Matrix3 &tm, Ogre::Vector3 &tv)
{
	//return texture mapping matrix and vector
	tm = t_matrix;
	tv = t_vector;
}

Ogre::SubMesh* Polygon::GetSubMesh()
{
	//return the submesh this polygon is in
	int index = mesh->FindMatchingSubMesh(material);
	return mesh->Submeshes[index];
}

Polygon::Polygon()
{
	mesh = 0;
}

Polygon::~Polygon()
{
}

void Polygon::GetGeometry(std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly, bool convert, bool rescale, bool relative, bool reverse)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	//if firstonly is true, only return first result
	//if convert is true, converts vertices from remote Ogre positions to local SBS positions
	//if rescale is true (along with convert), rescales vertices with UnitScale multiplier
	//if relative is true, vertices are relative of mesh center, otherwise they use absolute/global positioning
	//if reverse is false, process extents table in ascending order, otherwise descending order

	vertices.resize(index_extents.size());

	Ogre::Vector3 mesh_position;
	if (convert == true)
		mesh_position = mesh->GetPosition();
	else
		mesh_position = sbs->ToRemote(mesh->GetPosition());

	for (int i = 0; i < (int)index_extents.size(); i++)
	{
		int min = index_extents[i].x;
		int max = index_extents[i].y;
		int newsize = (int)vertices[i].size() + max - min + 1;
		vertices[i].reserve(newsize);
		if (reverse == false)
		{
			for (int j = min; j <= max; j++)
			{
				if (relative == true)
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex, rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex);
				}
				else
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex + mesh_position, rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex + mesh_position);
				}
			}
		}
		else
		{
			for (int j = max; j >= min; j--)
			{
				if (relative == true)
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex, rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex);
				}
				else
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex + mesh_position, rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex + mesh_position);
				}
			}
		}
		if (firstonly == true)
			return;
	}
}

bool Polygon::PointInside(const Ogre::Vector3 &point, bool plane_check, bool convert)
{
	//check if a point is inside the polygon

	std::vector<std::vector<Ogre::Vector3> > vertices;
	GetGeometry(vertices, false, convert);

	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (sbs->InPolygon(vertices[i], point))
			return true;
	}

	return false;
}

void Polygon::Move(const Ogre::Vector3 &position, float speed)
{
	for (int i = 0; i < (int)index_extents.size(); i++)
	{
		int min = index_extents[i].x;
		int max = index_extents[i].y;
		for (int j = min; j <= max; j++)
			mesh->MeshGeometry[j].vertex += sbs->ToRemote(position);
	}
}
