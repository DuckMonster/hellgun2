#pragma once
#include "gfx/render_info.h"
#include "core/callback.h"

struct Mesh;
struct Material;
struct Texture;
class Entity;

enum Drawable_Flag
{
	DRW_Disabled = 1 << 0,
	DRW_UI = 1 << 1,
	DRW_Blend = 1 << 2,
};

class Drawable
{
public:
	void load(const String& mesh_path, const String& material_path, const String& texture_path = String());
	void render(const Render_Info& info);
	void attach_to(Entity* entity) { attach_entity = entity; }
	Entity* attach_entity = nullptr;

	Mat4 get_world_matrix();

	Mesh* mesh = nullptr;
	Material* material = nullptr;
	Texture* texture = nullptr;

	Mat4 matrix = Mat4::identity;

	float depth_offset = 0.f;
	u32 flags = 0;

	void set_disabled(bool disabled)
	{
		if (disabled)
			flags |= DRW_Disabled;
		else
			flags &= ~DRW_Disabled;
	}

	Callback<const Render_Info&> on_render;
};