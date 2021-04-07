#ifndef _RENDER_OBJECT_H_
#define _RENDER_OBJECT_H_

#include <vector>
#include "../common/math.h"
#include <glm/gtc/matrix_transform.hpp>

class Texture;
class Shader;

class RenderObject
{

public:
	struct VertexAttr
	{
		enum class ElementType : int
		{
			Float = 0,
			Int
		};
		size_t element_count;
		ElementType element_type;
		bool normalization;
	};
	typedef std::vector<VertexAttr> VertexFormat;
	
	RenderObject(const VertexFormat& vertex_format, const void* vertex_data, size_t vertex_count, const unsigned int* indices, size_t index_count);
	~RenderObject();

	void render() const;

	// void renderTexture() const;

	Matrix4 get_model_matrix() const { return glm::translate(glm::mat4(1.0f), m_position); }
	
	Shader* getShader() const { return m_shader; }
	void setShader(Shader* shader) { m_shader = shader; }

	const Vector3& getPosition() const { return m_position; }
	void setPosition(const Vector3& position) { m_position = position; }

	void setPositionIndex(const int index) { m_position_index = index; }
	
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
	size_t m_vertex_count;
	size_t m_index_count;

	Shader* m_shader;
	Vector3 m_position;

	int m_position_index;
};
#endif