#pragma once
#include "entity/entity.h"

class Bullet : public Entity
{
	ENTITY_CLASS(Bullet)

public:
	Vec3 velocity;

	void update() override;
	void render(const Render_Info& info) override;
};