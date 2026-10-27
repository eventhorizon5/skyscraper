/* Local render light selection. Copyright (C)2026 Ryan Thoryk.
 * Licensed under the GPL, version 2 or later. */
#ifndef _SBS_LOCALLIGHTS_H
#define _SBS_LOCALLIGHTS_H

#include <OgreSceneManager.h>
#include <OgreLight.h>
#include <OgreMovableObject.h>
#include <algorithm>

namespace SBS {

// Each render entity gets a bounded, local light list. In particular, the
// container node can be far from the geometry, so use the world bounds center.
struct LocalLightSelector : Ogre::MovableObject::Listener
{
	Ogre::SceneManager *scene;
	int budget;
	Ogre::LightList lights;
	Ogre::AxisAlignedBox previous_bounds;
	unsigned long previous_frame = ~0UL;
	unsigned int previous_mask = 0;

	LocalLightSelector(Ogre::SceneManager *scene, int budget) : scene(scene), budget(budget) {}

	const Ogre::LightList* objectQueryLights(const Ogre::MovableObject *object) override
	{
		const auto &bounds = object->getWorldBoundingBox(true);
		unsigned long frame = scene->_getLightsDirtyCounter();
		if (frame == previous_frame && bounds == previous_bounds && object->getLightMask() == previous_mask)
			return &lights;
		previous_frame = frame;
		previous_bounds = bounds;
		previous_mask = object->getLightMask();
		lights.clear();
		if (bounds.isNull() || bounds.isInfinite())
			return &lights;

		Ogre::Vector3 center = bounds.getCenter();
		scene->_populateLightList(center, bounds.getHalfSize().length(), lights, previous_mask);
		// Texture shadow receivers require the shadow lights in OGRE's order.
		// For the normal/stencil path, sort even shadow-casting lights locally.
		if (!scene->isShadowTechniqueTextureBased())
		{
			lights.erase(std::remove_if(lights.begin(), lights.end(), [&](const Ogre::Light *light)
			{
				return !light->isInLightRange(bounds);
			}), lights.end());
			std::stable_sort(lights.begin(), lights.end(), [&](const Ogre::Light *a, const Ogre::Light *b)
			{
				bool ad = a->getType() == Ogre::Light::LT_DIRECTIONAL;
				bool bd = b->getType() == Ogre::Light::LT_DIRECTIONAL;
				if (ad != bd)
					return ad;
				if (ad)
					return false;
				return a->getDerivedPosition().squaredDistance(center) < b->getDerivedPosition().squaredDistance(center);
			});
		}
		if (lights.size() > static_cast<size_t>(budget))
			lights.resize(budget);
		return &lights;
	}
};

}
#endif
