#include "player.h"
#include "core/input.h"
#include "math/matrix_math.h"
#include "game.h"
#include "debug/debug.h"

static const Vec2 PLAYER_MESH_DATA[] = {
	{ -0.5f, -0.5f },
	{ 0.5f, -0.5f },
	{ 0.5f, 0.5f },

	{ -0.5f, -0.5f },
	{ 0.5f, 0.5f },
	{ -0.5f, 0.5f },
};

void Player::init()
{
	mesh.init();
	mesh.add_buffer(0);
	mesh.bind_buffer(0, 2);
	mesh.buffer_data(0, sizeof(PLAYER_MESH_DATA), PLAYER_MESH_DATA);

	mesh.draw_mode = GL_TRIANGLES;
	mesh.draw_num = 6;

	mat.load_file("res/test.vert", "res/test.frag");
}

void Player::update()
{
	float spd = 2.f;
	if (key_down(Key::LeftShift))
		spd = 10.f;

	if (key_down(Key::A))
	{
		if (key_down(Key::LeftAlt))
			position.x -= spd * time_delta();
		else
			velocity.x -= spd * time_delta();
	}
	if (key_down(Key::D))
	{
		if (key_down(Key::LeftAlt))
			position.x += spd * time_delta();
		else
			velocity.x += spd * time_delta();
	}
	if (key_down(Key::S))
	{
		if (key_down(Key::LeftAlt))
			position.y -= spd * time_delta();
		else
			velocity.y -= spd * time_delta();
	}
	if (key_down(Key::W))
	{
		if (key_down(Key::LeftAlt))
			position.y += spd * time_delta();
		else
			velocity.y += spd * time_delta();
	}

	if (key_down(Key::E))
		velocity.z += spd * time_delta();
	if (key_down(Key::Q))
		velocity.z -= spd * time_delta();


	/*
	if (key_down(Key::A))
		input.x -= 1.f;
	if (key_down(Key::D))
		input.x += 1.f;

	if (key_down(Key::Spacebar))
	{
		velocity.y = 10.f;
	}

	velocity.x += input.x * PLAYER_WALK_ACC * time_delta();
	velocity += -Vec3::up * PLAYER_GRAVITY * time_delta();
	velocity -= velocity * PLAYER_FRICTION * time_delta();

	position += velocity * time_delta();

	if (position.y <= -5.f)
	{
		position.y = -5.f;
		velocity.y = -velocity.y * 0.5f;
	}
	*/
}

void Player::render(const Render_Info& info)
{
	mat.use();
	mat.set("u_ViewProjection", info.view_projection);
	mat.set("u_Model", mat_translation(position));

	mesh.bind();
	mesh.draw();
}