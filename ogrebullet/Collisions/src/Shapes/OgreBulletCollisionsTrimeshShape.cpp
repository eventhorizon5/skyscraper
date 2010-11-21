/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreBulletCollisions.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"
#include "Utils/OgreBulletConverter.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    TriangleMeshCollisionShape::TriangleMeshCollisionShape(
        Vector3        *vertices, 
        unsigned int vertexCount, 
        unsigned int *indices, 
        unsigned int indexCount,
		bool use32bitsIndices) :	
        CollisionShape(),
        mTriMesh(0)
    {
		unsigned int numFaces = indexCount / 3;

		mTriMesh = new btTriangleMesh(use32bitsIndices);
		mTriMesh->preallocateVertices(vertexCount);
		mTriMesh->preallocateIndices(indexCount);

        btVector3    vertexPos[3];
        for (unsigned int n = 0; n < numFaces; ++n)
        {
			{
				const Vector3 &vec = vertices[*indices];
				vertexPos[0][0] = vec.x;
				vertexPos[0][1] = vec.y;
				vertexPos[0][2] = vec.z;
			}
			{
				const Vector3 &vec = vertices[*(indices + 1)];
				vertexPos[1][0] = vec.x;
				vertexPos[1][1] = vec.y;
				vertexPos[1][2] = vec.z;
			}
			{
				const Vector3 &vec = vertices[*(indices + 2)];
				vertexPos[2][0] = vec.x;
				vertexPos[2][1] = vec.y;
				vertexPos[2][2] = vec.z;
			}

			indices += 3;

            mTriMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2]);
        }

		const bool useQuantizedAABB = true;
        mShape = new btBvhTriangleMeshShape(mTriMesh, useQuantizedAABB);

    }

	//this constructor allows for adding triangles individually at a later stage.
	//sequence is this function, then AddTriangle(), and then Finish()
    TriangleMeshCollisionShape::TriangleMeshCollisionShape(
        unsigned int vertexCount, 
        unsigned int indexCount, 
		bool use32bitsIndices) :	
        CollisionShape(),
        mTriMesh(0)
    {
		mTriMesh = new btTriangleMesh(use32bitsIndices);
		mTriMesh->preallocateVertices(vertexCount);
		mTriMesh->preallocateIndices(indexCount);
    }

	//add an individual triangle
    void TriangleMeshCollisionShape::AddTriangle(
        Vector3        &vertex1, 
        Vector3        &vertex2, 
        Vector3        &vertex3)
    {
        btVector3    vertexPos[3];

		vertexPos[0][0] = vertex1.x;
		vertexPos[0][1] = vertex1.y;
		vertexPos[0][2] = vertex1.z;
		vertexPos[1][0] = vertex2.x;
		vertexPos[1][1] = vertex2.y;
		vertexPos[1][2] = vertex2.z;
		vertexPos[2][0] = vertex3.x;
		vertexPos[2][1] = vertex3.y;
		vertexPos[2][2] = vertex3.z;

        mTriMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2]);
    }

	//finalize collider
	void TriangleMeshCollisionShape::Finish()
	{
		const bool useQuantizedAABB = true;
        mShape = new btBvhTriangleMeshShape(mTriMesh, useQuantizedAABB);
	}

	// -------------------------------------------------------------------------
    TriangleMeshCollisionShape::~TriangleMeshCollisionShape()
    {
        if(mTriMesh)
        {
            delete mTriMesh;
        }
        mTriMesh = 0;
    }
    // -------------------------------------------------------------------------
	bool TriangleMeshCollisionShape::drawWireFrame(DebugLines *wire, 
		const Ogre::Vector3 &pos, 
		const Ogre::Quaternion &quat) const
    {
        const int numTris = mTriMesh->getNumTriangles ();
        if (numTris > 0)
        {

			const int numSubParts = mTriMesh->getNumSubParts ();
			for (int currSubPart = 0; currSubPart < numSubParts; currSubPart++)
			{
				const unsigned char* vertexBase = NULL;
				int numVerts;
				PHY_ScalarType vertexType;
				int vertexStride;
				const unsigned char* indexBase = NULL;
				int indexStride;
				int numFaces;
				PHY_ScalarType indexType;

				mTriMesh->getLockedReadOnlyVertexIndexBase (&vertexBase, numVerts, 
					vertexType, vertexStride, 
					&indexBase, indexStride, numFaces, indexType, currSubPart);

				float* p;
				btVector3 vert0;
				btVector3 vert1;
				btVector3 vert2;
				for (int t = 0; t < numFaces; t++)
				{
#define setVector(A, B) {A.setX(B[0]);A.setY(B[1]);A.setZ(B[2]);};

					if (indexType == PHY_SHORT)
					{
						short int* index = (short int*)(indexBase + t*indexStride);

						p = (float*)(vertexBase + index[0]*vertexStride);
						setVector(vert0, p);						
						p = (float*)(vertexBase + index[1]*vertexStride);
						setVector(vert1, p);			
						p = (float*)(vertexBase + index[2]*vertexStride);
						setVector(vert2, p);		
					} 
					else
					{
						int* index = (int*)(indexBase + t*indexStride);

						p = (float*)(vertexBase + index[0]*vertexStride);
						setVector(vert0, p);						
						p = (float*)(vertexBase + index[1]*vertexStride);
						setVector(vert1, p);			
						p = (float*)(vertexBase + index[2]*vertexStride);
						setVector(vert2, p);		
					}
#undef setVector

					wire->addLine (BtOgreConverter::to(vert0), BtOgreConverter::to(vert1));
					wire->addLine (BtOgreConverter::to(vert1), BtOgreConverter::to(vert2));
					wire->addLine (BtOgreConverter::to(vert2), BtOgreConverter::to(vert0));
				}
			}
            return true;
        }
        return false;
    }
}

