#ifndef _OGREBULLET_HEIGHTMAP_
#define _OGREBULLET_HEIGHTMAP_


#include "OgreBulletCollisionsPreRequisites.h"

#include "OgreBulletCollisionsShape.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"

namespace OgreBulletCollisions
{
	
	class DebugHelper : public btIDebugDraw
	{
	public:

		DebugHelper(DebugLines* pLines) : m_pLines(pLines) {}
		~DebugHelper () {}

		void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
		{
			Ogre::Vector3 a(from.x(), from.y(), from.z());
			Ogre::Vector3 b(to.x(), to.y(), to.z());
			m_pLines->addLine(a, b);
		}
		void	draw3dText(const btVector3 &,const char *)
		{

		}

		void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
		{
		}

		void	reportErrorWarning(const char* warningString)
		{
		}

		void	setDebugMode(int debugMode)
		{
		}

		int		getDebugMode() const
		{
			return -1;
		}

	private:
		DebugLines* m_pLines;
	};

	class DebugTriangleDrawCallback : public btTriangleCallback
	{
	private:

		DebugHelper     *mDebugHelper;
		btTransform     mTransform;
		btVector3		mColor;

	public:

		DebugTriangleDrawCallback(DebugHelper *db, btTransform &bt, const btVector3& color) :
		  btTriangleCallback(),
			mDebugHelper(db),
			mTransform(bt),
			mColor(color)
		{

		}

		void processTriangle(btVector3* triangle, int partId, int triangleIndex)
		{
			mDebugHelper->drawLine (*triangle,     *(triangle+1), mColor);
			mDebugHelper->drawLine (*(triangle+1), *(triangle+2), mColor);
			mDebugHelper->drawLine (*(triangle+2), *triangle,     mColor);
		}						 
	};

	class HeightmapCollisionShape : public CollisionShape
	{
	public:
		HeightmapCollisionShape(int width, int length, Ogre::Vector3& scale, Ogre::Real* pHeightData, bool bFlip)
		{
			int upIndex = 1;
			bool useFloatDatam=true;
			bool flipQuadEdges=bFlip;

			btHeightfieldTerrainShape* pHeightShape = 
				new btHeightfieldTerrainShape(width, length, pHeightData, scale.y, upIndex, useFloatDatam, flipQuadEdges);
			pHeightShape->setUseDiamondSubdivision(true);

			mShape = pHeightShape;

			btVector3 sc(scale.x, scale.y, scale.z);
			mShape->setLocalScaling(sc);

		}

		virtual ~HeightmapCollisionShape()
		{
		}

		bool drawWireFrame(DebugLines *wire, 
			const Ogre::Vector3 &pos = Ogre::Vector3::ZERO, 
			const Ogre::Quaternion &quat= Ogre::Quaternion::IDENTITY) const
		{
			btHeightfieldTerrainShape* pHeightShape = static_cast<btHeightfieldTerrainShape*>(mShape);

			btTransform bt;
			bt.setIdentity();

			btVector3 colour(255.0, 255.0, 255.0);

			DebugHelper ddraw(wire);
			DebugTriangleDrawCallback cb(&ddraw, bt, colour);

			btVector3 aabbMax(btScalar(1e30),btScalar(1e30),btScalar(1e30));
			btVector3 aabbMin(btScalar(-1e30),btScalar(-1e30),btScalar(-1e30));
			pHeightShape->processAllTriangles(&cb, aabbMin, aabbMax);
			return true;
		}

	};
}


#endif // _OGREBULLET_HEIGHTMAP_