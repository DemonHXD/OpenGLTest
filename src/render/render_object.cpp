#include "glad/glad.h"
#include "render_object.h"
#include <cassert>
#include "texture.h"
#include "shader.h"
#include <iostream>
#include "../engine/engine.h"
#include "../engine/camera.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include <string.h>

void RenderObject::setRenderObject(const std::string vaoName, const VertexFormat &vertex_format, const void *vertex_data, size_t vertex_count, const unsigned *indices, size_t index_count)
{
	unsigned int vao;
	std::vector<unsigned> offsets;
	unsigned int vertex_size = 0;
	for (const auto &attr : vertex_format)
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
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &m_vbo);
	m_vertex_count = vertex_count;
	m_index_count = 0;

	glBindVertexArray(vao);
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
		const auto &attr = vertex_format[i];
		auto element_type = GL_FLOAT;
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Int:
			element_type = GL_INT;
			break;
		default:
			break;
		}
		glVertexAttribPointer(i, attr.element_count, element_type, attr.normalization, vertex_size, (void *)(offsets[i]));
		glEnableVertexAttribArray(i);
	}
	m_vaos.insert(std::pair<std::string, unsigned int>(vaoName, vao));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderObject::setRenderObject(const std::string vaoName, size_t vertex_count)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_count * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	m_vaos.insert(std::pair<std::string, unsigned int>(vaoName, vao));
}

RenderObject::~RenderObject()
{
	std::map<std::string, unsigned int>::iterator it;
	for (it = m_vaos.begin(); it != m_vaos.end(); it++)
	{
		glDeleteVertexArrays(1, &it->second);
	}
	glDeleteBuffers(1, &m_vbo);
	if (m_index_count > 0)
	{
		glDeleteBuffers(1, &m_ebo);
	}
}

void RenderObject::setPosition(unsigned int positionCount, Vector3 positions[])
{
	for (int index = 0; index < positionCount; index++)
	{
		m_positions.push_back(positions[index]);
	}
}

void RenderObject::render() const
{
	Engine &engine = Engine::get_singleton();
	Camera *camera = engine.getCamera();
	TextureManager &textureManager = TextureManager::get_singleton();
	ShaderManager &shaderManager = ShaderManager::get_singleton();

	Shader *lightingShader = shaderManager.getShaders().at("lightingShader");
	Shader *lightCubeShader = shaderManager.getShaders().at("lightCubeShader");

	unsigned int cubeVAO = m_vaos.at("cubeVAO");
	unsigned int lightCubeVAO = m_vaos.at("lightCubeVAO");

	lightingShader->bind();
	// 设置光照属性
	lightingShader->setVec3("light.position", camera->getPosition());
	lightingShader->setVec3("light.direction", camera->getFront());
	lightingShader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
	lightingShader->setVec3("viewPos", camera->getPosition());


	lightingShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	lightingShader->setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	lightingShader->setFloat("light.constant", 1.0f);
	lightingShader->setFloat("light.linear", 0.09f);
	lightingShader->setFloat("light.quadratic", 0.032f);

	lightingShader->setFloat("material.shininess", 32.0f);

	glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)800.0f / (float)600.0f, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	lightingShader->setMat4("projection", projection);
	lightingShader->setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	lightingShader->setMat4("model", model);

	textureManager.renderAllTexture(lightCubeShader);

	// render containers
	glBindVertexArray(cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_positions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightingShader->unbind();
	lightCubeShader->unbind();
}