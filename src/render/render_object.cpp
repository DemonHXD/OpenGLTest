#include "glad/glad.h"
#include <cassert>
#include <GLFW/glfw3.h>
#include <iostream>

#include "render_object.h"
#include "texture.h"
#include "shader.h"
#include "model.h"
#include "shader_manager.h"
#include "model_manager.h"

#include "../engine/engine.h"
#include "../engine/camera.h"
#include "../common/lib_utils.h"

#include <string.h>

RenderObject::RenderObject()
{
	JsonLoad::getInstance()->getJsonData(m_model_light_data, "model_light");
}

void RenderObject::setRenderObject(const std::string vaoName, const VertexFormat &vertex_format, const void *vertex_data, size_t vertex_count, const unsigned int *indices, size_t index_count)
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

void RenderObject::setRenderObject(const std::string vaoName, const void *vertex_data, size_t vertex_count)
{
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float) * 36, vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_count * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	m_vaos.insert(std::pair<std::string, unsigned int>(vaoName, vao));
}

void RenderObject::setRenderObject(std::vector<Model::MeshVertex> meshVertexs)
{
	for (unsigned int i = 0; i < meshVertexs.size(); i++)
	{
		unsigned int vao, vbo, ebo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, meshVertexs[i].vertices.size() * sizeof(Model::ModelVertex), &meshVertexs[i].vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshVertexs[i].indices.size() * sizeof(unsigned int), &meshVertexs[i].indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::ModelVertex), (void *)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::ModelVertex), (void *)offsetof(Model::ModelVertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::ModelVertex), (void *)offsetof(Model::ModelVertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Model::ModelVertex), (void *)offsetof(Model::ModelVertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Model::ModelVertex), (void *)offsetof(Model::ModelVertex, Bitangent));

		glBindVertexArray(0);
		m_model_texturesName.push_back(meshVertexs[i].texturesName);
		m_model_textures.push_back(meshVertexs[i].textures);
		m_model_vaos.insert(std::pair<std::string, unsigned int>("model_vao_" + std::to_string(i), vao));
	}
	m_meshVertexs = meshVertexs;
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

void RenderObject::setUserDataVector3s(std::string vectorName, std::vector<Vector3> vector3s)
{
	m_map_userData.insert(std::pair<std::string, std::vector<Vector3>>(vectorName, vector3s));
}

void RenderObject::setPointLightPositions(unsigned int positionCount, Vector3 pointLightPositions[])
{
	for (int index = 0; index < positionCount; index++)
	{
		m_point_light_positions.push_back(pointLightPositions[index]);
	}
}

void RenderObject::renderCube()
{
	Camera *camera = Engine::get_singletonPtr()->getCamera();
	ShaderManager &shaderManager = ShaderManager::get_singleton();
	Shader *shader = shaderManager.getShaders().at("shader");
	unsigned int boxCubeVAO = m_vaos.at("boxCubeVAO");
	unsigned int planeVAO = m_vaos.at("planeVAO");
	unsigned int transparentVAO = m_vaos.at("transparentVAO");

	auto windows = m_map_userData.at("windows");
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < windows.size(); i++)
	{
		float distance = glm::length(camera->getPosition() - windows[i]);
		sorted[distance] = windows[i];
	}

	shader->bind();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	// cubes
	glBindVertexArray(boxCubeVAO);
	shader->renderTexture("marble.jpg", "texture1");
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// floor
	glBindVertexArray(planeVAO);
	shader->renderTexture("metal.png", "texture1");
	shader->setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// windows (from furthest to nearest)
	glBindVertexArray(transparentVAO);
	shader->renderTexture("blending_transparent_window.png", "texture1");
	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		shader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);
}

void RenderObject::renderModel()
{
}

void RenderObject::render()
{
	renderCube();
	// renderModel();
}