#ifndef _RENDER_H_
#define _RENDER_H_

#include "render_object.h"
#include "../common/singleton.h"
#include <vector>
#include <string>

class RenderObject;
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
	
	void addRenderObject(RenderObject* object);

protected:
	void begin_frame(float delta);
	void end_frame(bool swap_buffer);

private:
	std::vector<RenderObject*> _render_objects;
};
#endif