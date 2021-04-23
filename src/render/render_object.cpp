#include "glad/glad.h"
#include "render_object.h"
#include <cassert>
#include "texture.h"
#include "shader.h"
#include "model.h"
#include <iostream>
#include "../engine/engine.h"
#include "../engine/camera.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include "model_manager.h"
#include "model.h"
#include "../common/lib_utils.h"
#include <string.h>

RenderObject::RenderObject()
{
	m_dirLight_data = JsonLoad::getInstance()->getDirLightData();
	m_pointLights_data = JsonLoad::getInstance()->getPointLightsData();
	m_spotLightData_data = JsonLoad::getInstance()->getSpotLightData();
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

void RenderObject::setRenderObject(const std::string vaoName, const std::vector<ModelVertex> modelVertex, std::vector<unsigned int> modelIndices)
{
	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, modelVertex.size() * sizeof(ModelVertex), &modelVertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelIndices.size() * sizeof(unsigned int), &modelIndices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)offsetof(ModelVertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)offsetof(ModelVertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)offsetof(ModelVertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)offsetof(ModelVertex, Bitangent));

	glBindVertexArray(0);
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

void RenderObject::setPointLightPositions(unsigned int positionCount, Vector3 pointLightPositions[])
{
	for (int index = 0; index < positionCount; index++)
	{
		m_point_light_positions.push_back(pointLightPositions[index]);
	}
}

void RenderObject::renderCube()
{
	Engine &engine = Engine::get_singleton();
	Camera *camera = engine.getCamera();
	ShaderManager &shaderManager = ShaderManager::get_singleton();

	Shader *lightingShader = shaderManager.getShaders().at("lightingShader");
	Shader *lightCubeShader = shaderManager.getShaders().at("lightCubeShader");

	unsigned int cubeVAO = m_vaos.at("cubeVAO");
	unsigned int lightCubeVAO = m_vaos.at("lightCubeVAO");

	lightingShader->bind();

	lightingShader->setVec3("viewPos", camera->getPosition());
	lightingShader->setFloat("material.shininess", 32.0f);
	// directional light
	lightingShader->setVec3("dirLight.direction", m_dirLight_data.direction);
	lightingShader->setVec3("dirLight.ambient", m_dirLight_data.ambient);
	lightingShader->setVec3("dirLight.diffuse", m_dirLight_data.diffuse);
	lightingShader->setVec3("dirLight.specular", m_dirLight_data.specular);

	for (unsigned int i = 0; i < m_point_light_positions.size(); i++)
	{
		std::string argsName = LibUtils::getInstance()->formatString("pointLights[%d]", i);
		lightingShader->setVec3(argsName + ".position", m_point_light_positions[i]);
		lightingShader->setVec3(argsName + ".ambient", m_pointLights_data.ambient);
		lightingShader->setVec3(argsName + ".diffuse", m_pointLights_data.diffuse);
		lightingShader->setVec3(argsName + ".specular", m_pointLights_data.specular);
		lightingShader->setFloat(argsName + ".constant", m_pointLights_data.constant);
		lightingShader->setFloat(argsName + ".linear", m_pointLights_data.linear);
		lightingShader->setFloat(argsName + ".quadratic", m_pointLights_data.quadratic);
	}

	// spotLight
	lightingShader->setVec3("spotLight.position", camera->getPosition());
	lightingShader->setVec3("spotLight.direction", camera->getFront());
	lightingShader->setVec3("spotLight.ambient", m_spotLightData_data.ambient);
	lightingShader->setVec3("spotLight.diffuse", m_spotLightData_data.diffuse);
	lightingShader->setVec3("spotLight.specular", m_spotLightData_data.specular);
	lightingShader->setFloat("spotLight.constant", m_spotLightData_data.constant);
	lightingShader->setFloat("spotLight.linear", m_spotLightData_data.linear);
	lightingShader->setFloat("spotLight.quadratic", m_spotLightData_data.quadratic);
	lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	Matrix4 projection = glm::perspective(glm::radians(camera->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
	Matrix4 view = camera->GetViewMatrix();
	lightingShader->setMat4("projection", projection);
	lightingShader->setMat4("view", view);
	Matrix4 model = Matrix4(1.0f);
	lightingShader->setMat4("model", model);

	// 为lightingShader渲染所有纹理
	lightingShader->renderTextures();
	// textureManager.render

	// 渲染十个箱子
	glBindVertexArray(cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		Matrix4 model = Matrix4(1.0f);
		model = glm::translate(model, m_positions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), Vector3(1.0f, 0.3f, 0.5f));
		lightingShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightCubeShader->bind();
	lightCubeShader->setMat4("projection", projection);
	lightCubeShader->setMat4("view", view);
	// 渲染四个发光体
	glBindVertexArray(lightCubeVAO);
	for (unsigned int i = 0; i < 4; i++)
	{
		model = Matrix4(1.0f);
		model = glm::translate(model, m_point_light_positions[i]);
		model = glm::scale(model, Vector3(0.2f));
		lightCubeShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightingShader->unbind();
	lightCubeShader->unbind();
}

void RenderObject::renderModel()
{
	Engine &engine = Engine::get_singleton();
	Camera *camera = engine.getCamera();
	ShaderManager &shaderManager = ShaderManager::get_singleton();
	ModelManager &modelManager = ModelManager::get_singleton();

	Shader *ourShader = shaderManager.getShaders().at("ourShader");
	Model *ourModel = modelManager.getModels().at("ourModel");
	unsigned int modelVAO = m_vaos.at("modelVAO");
	ourShader->bind();
	glBindVertexArray(modelVAO);
	ourShader->renderTextures(ourModel->getModelTexturesName());
	
	// view/projection transformations
	Matrix4 projection = glm::perspective(glm::radians(camera->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
	Matrix4 view = camera->GetViewMatrix();
	ourShader->setMat4("projection", projection);
	ourShader->setMat4("view", view);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Vector3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, Vector3(0.2f, 0.2f, 0.2f));		// it's a bit too big for our scene, so scale it down
	ourShader->setMat4("model", model);

	glDrawElements(GL_TRIANGLES, ourModel->getModelIndices().size(), GL_UNSIGNED_INT, 0);

	ourShader->unbind();
	glBindVertexArray(0);
}

void RenderObject::render()
{
	renderCube();
	renderModel();
}