#include <glad/glad.h>
#include "render.h"

Render *Singleton<Render>::singleton = nullptr;

Render::~Render()
{
	for (auto object : _render_objects)
	{
		delete object;
	}
}

void Render::begin_frame(float delta)
{
	//渲染指令……
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::end_frame(bool swap_buffer)
{
	// TODO swap buffer
}

void Render::draw(float delta)
{
	begin_frame(delta);

	for (auto object : _render_objects)
	{
		object->render();
	}

	end_frame(true);
}

void Render::addRenderObject(RenderObject *object)
{
	_render_objects.emplace_back(object);
}

