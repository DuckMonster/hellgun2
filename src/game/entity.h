#pragma once
#include "renderinfo.h"

class Entity
{
public:
	Vec3 position;

	virtual void init() {}
	virtual void update() {}
	virtual void render(const Render_Info& info) {}
};