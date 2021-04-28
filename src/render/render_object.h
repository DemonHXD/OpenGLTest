#ifndef _RENDER_OBJECT_H_
#define _RENDER_OBJECT_H_

#include <vector>
#include <map>
#include "../common/math.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../common/json_load.h"
#include "model.h"

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

	RenderObject();
	~RenderObject();

	void setRenderObject(const std::string vaoName, const VertexFormat &vertex_format, const void *vertex_data, size_t vertex_count, const unsigned int *indices, size_t index_count);
	void setRenderObject(const std::string vaoName, const void *vertex_data, size_t vertex_count);
	void setRenderObject(std::vector<Model::MeshVertex> meshVertexs);
	void render();

	Matrix4 get_model_matrix(Vector3 position) const { return glm::translate(glm::mat4(1.0f), position); }

	// const std::vector<Vector3> getPositions() const { return m_positions; }
	void setUserDataVector3s(std::string vectorName, std::vector<Vector3> vector3s);

	const std::vector<Vector3> getPointLightPositions() const { return m_point_light_positions; }
	void setPointLightPositions(unsigned int positionCount, Vector3 pointLightPositions[]);

	void setPositionIndex(const int index) { m_position_index = index; }

private:

	void renderCube();
	void renderModel();

	std::map<std::string, unsigned int> m_vaos;
	unsigned int m_vbo;
	unsigned int m_ebo;
	size_t m_vertex_count;
	size_t m_index_count;

	Vector3 m_lightPos = Vector3(1.2f, 1.0f, 2.0f);

	// std::vector<Vector3> m_positions;
	std::map<std::string, std::vector<Vector3>> m_map_userData;
	std::vector<Vector3> m_point_light_positions;
	int m_position_index;

	JsonLoad::PointLightsData m_model_light_data;

	std::vector<Model::MeshVertex> m_meshVertexs;
	std::vector<std::vector<std::string>> m_model_texturesName;
	std::vector<std::vector<Texture*>> m_model_textures;
	std::map<std::string, unsigned int> m_model_vaos;
};
#endif