#ifndef _RENDER_H_
#define _RENDER_H_

#include "render_object.h"
#include "../common/singleton.h"
#include <vector>

class Render : public Singleton<Render>
{
public:
	Render() = default;
	~Render();

	Render(const Render&) = delete;
	Render(Render&&) = delete;
	Render& operator=(const Render&) = delete;
	Render& operator=(Render&&) = delete;

	void draw(float delta);
	
	RenderObject* add_renderable(const RenderObject::VertexFormat& vformat, const void* vertices, size_t vcount, const unsigned int* indices = nullptr, size_t icount = 0);

protected:
	void begin_frame(float delta);
	void end_frame(bool swap_buffer);

private:
	std::vector<RenderObject*> _render_objects;
};
#endif