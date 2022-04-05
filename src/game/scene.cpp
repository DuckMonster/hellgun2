#include "scene.h"

Scene scene;

Collider* Scene::add_collider()
{
	return &colliders.add_default();
}