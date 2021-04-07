#include "glad/glad.h"
#include "render_object.h"
#include <cassert>
#include "texture.h"
#include "shader.h"
#include <iostream>
#include "../engine/engine.h"
#include "../engine/camera.h"
#include "texture_manager.h"
#include <string.h>

RenderObject::RenderObject(const VertexFormat& vertex_format, const void* vertex_data, size_t vertex_count, const unsigned* indices, size_t index_count)
{
	std::vector<unsigned> offsets;
	unsigned int vertex_size = 0;
	for (const auto& attr : vertex_format)
	{
		offsets.emplace_back(vertex_size);
		size_t element_size = sizeof(float);
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Int:
			element_size = sizeof(int);
			break;
		default:
			break;
		}
		vertex_size += element_size * attr.element_count;
	}
	assert(vertex_count >= 3);
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	m_vertex_count = vertex_count;
	m_index_count = 0;
	
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * vertex_count, vertex_data, GL_STATIC_DRAW);

	assert(index_count % 3 == 0);
	if (indices != nullptr && index_count > 0)
	{
		m_index_count = index_count;
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * index_count, indices, GL_STATIC_DRAW);
	}

	for (size_t i = 0; i < vertex_format.size(); ++i)
	{
		const auto& attr = vertex_format[i];
		auto element_type = GL_FLOAT;
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Int:
			element_type = GL_INT;
			break;
		default:
			break;
		}
		glVertexAttribPointer(i, attr.element_count, element_type, attr.normalization, vertex_size, (void*)(offsets[i]));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

RenderObject::~RenderObject()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	if (m_index_count > 0)
	{
		glDeleteBuffers(1, &m_ebo);
	}
}

void RenderObject::render() const
{
	Engine& engine = Engine::get_singleton();
	Camera* camera = engine.getCamera();
	TextureManager& textureManager = TextureManager::get_singleton();
	
	
	m_shader->bind();
	textureManager.renderAllTexture(m_shader);
	
	//定义投影矩阵
	Matrix4 projection = glm::perspective(glm::radians(engine.getCamera()->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("view", engine.getCamera()->GetViewMatrix());

	Matrix4 model = Matrix4(1.0f);
	model = glm::translate(model, m_position);
	float angle = 20.0f * m_position_index;
	float timeOrRadians = (m_position_index < 1 || m_position_index % 3 == 0) ? engine.get_time() : glm::radians(angle);
	model = glm::rotate(model, timeOrRadians, Vector3(1.0f, 0.3f, 0.5f));
	m_shader->setMat4("model", model);
	
	glBindVertexArray(m_vao);
	if (m_index_count > 0)
	{
		glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
	}

	m_shader->unbind();
}