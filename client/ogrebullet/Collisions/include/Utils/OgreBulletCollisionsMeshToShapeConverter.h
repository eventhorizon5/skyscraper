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
#ifndef _OgreBulletCollisionsMeshToShapeConverter_H_
#define _OgreBulletCollisionsMeshToShapeConverter_H_

#include "OgreBulletCollisionsPreRequisites.h"

namespace OgreBulletCollisions 
{
	class _OgreBulletExport VertexIndexToShape
	{
	public:
		VertexIndexToShape(const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		~VertexIndexToShape();

		Ogre::Real      getRadius();
		Ogre::Vector3   getSize();


		SphereCollisionShape*           createSphere();
		BoxCollisionShape*              createBox();
		TriangleMeshCollisionShape*     createTrimesh();
		CylinderCollisionShape*         createCylinder();
		ConvexHullCollisionShape*       createConvex();
		GImpactConcaveShape*			createConcave();
		CompoundCollisionShape*		createConvexDecomposition(
		  unsigned int depth = 5 // depth to split, a maximum of 10, generally not over 7.
		, float cpercent     = 5 	// the concavity threshold percentage.  0=20 is reasonable.
		, float ppercent     = 15 // the percentage volume conservation threshold to collapse hulls. 0-30 is reasonable.
		, unsigned int maxv  = 32 // maximum number of vertices in the output hull. Recommended 32 or less.
		, float skinWidth    = 0.0 // a skin width to apply to the output hulls.
			);


		const Ogre::Vector3*    getVertices();
		unsigned int            getVertexCount();
		const unsigned int*     getIndices();
		unsigned int            getIndexCount();

	protected:

		void addStaticVertexData(const Ogre::VertexData *vertex_data);

		void addAnimatedVertexData(const Ogre::VertexData *vertex_data, 
							const Ogre::VertexData *blended_data, 
							const Ogre::Mesh::IndexMap *indexMap);

		void addIndexData(Ogre::IndexData *data, const unsigned int offset = 0);

	
	protected:
		Ogre::Vector3*	    mVertexBuffer;
		unsigned int*       mIndexBuffer;
		unsigned int        mVertexCount;
		unsigned int        mIndexCount;

		Ogre::Matrix4		mTransform;

		Ogre::Real		    mBoundRadius;
		Ogre::Vector3		mBounds;

		BoneIndex           *mBoneIndex;

	};

	class _OgreBulletExport StaticMeshToShapeConverter : public VertexIndexToShape
	{
	public:

		StaticMeshToShapeConverter(Ogre::Renderable *rend, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		StaticMeshToShapeConverter(Ogre::Entity *entity,   const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		StaticMeshToShapeConverter();

		~StaticMeshToShapeConverter();

		virtual void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		
		virtual void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);


	protected:

		Ogre::Entity*		mEntity;
		Ogre::SceneNode*	mNode;

	};

	class _OgreBulletExport AnimatedMeshToShapeConverter : public VertexIndexToShape
	{
	public:

		AnimatedMeshToShapeConverter(Ogre::Entity *entity, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		AnimatedMeshToShapeConverter();
		~AnimatedMeshToShapeConverter();

		void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform);

		BoxCollisionShape* createAlignedBox(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation);

		BoxCollisionShape* createOrientedBox(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation);

		CapsuleCollisionShape* createOrientedCapsuleCollisionShape(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation);

	protected:

		bool getBoneVertices(unsigned char bone, 
			unsigned int &vertex_count, 
			Ogre::Vector3* &vertices,
			const Ogre::Vector3 &bonePosition);

		bool getOrientedBox(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation,
			Ogre::Vector3 &extents,
			Ogre::Vector3 *axis,
			Ogre::Vector3 &center);

		
		Ogre::Entity*		mEntity;
		Ogre::SceneNode*	mNode;

		Ogre::Vector3       *mTransformedVerticesTemp;
		size_t               mTransformedVerticesTempSize;

	};

}

#endif




