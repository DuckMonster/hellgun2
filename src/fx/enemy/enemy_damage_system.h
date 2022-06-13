#pragma once
#include "fx/particle_system.h"

struct Mesh;
struct Texture;
struct Materal;
class Drawable;

class Enemy_Damage_System : public Particle_System
{
private:
	Mesh* mesh;
	Texture* texture;
	Material* material;
	Mat4 transform;

public:
	void init(Drawable* drawable);
	void init(Mesh* mesh, Texture* texture, const Mat4& transform);

	void system_update();
	void system_render(const Render_Info& info);
};