#pragma once
#include "fx/particle_system.h"

class Muzzle_Flash_System : public Particle_System
{
public:
	void init(const Vec3& direction);
};