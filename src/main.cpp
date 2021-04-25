#include "engine/engine.h"
#include "common/math.h"
#include "render/texture.h"
#include "render/shader.h"
#include "render/render.h"
#include "render/render_object.h"
#include "render/shader_manager.h"
#include "render/model_manager.h"
#include "render/model.h"
#include <map>
using namespace std;

int main()
{

	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	std::shared_ptr<Render> render = std::make_shared<Render>();
	std::shared_ptr<ShaderManager> shaderManager = std::make_shared<ShaderManager>();
	std::shared_ptr<ModelManager> mondelManager = std::make_shared<ModelManager>();

	if (!engine->initWindow(800, 600))
	{
		return -1;
	}

	//Shader* lightingShader = new Shader();
	//if (!lightingShader->loadShaderAsset("light_vertexShader.vs", "light_fragmentShader.fs"))
	//{
	//	return -1;
	//}
	//lightingShader->setTextureNames(2, "material.diffuse", "material.specular");
	//textureManager->addLoadTexture(lightingShader, 2, "container2.png", "container2_specular.png");
	//
	//Shader* lightCubeShader = new Shader();
	//if (!lightCubeShader->loadShaderAsset("light_vertexShader2.vs", "light_fragmentShader2.fs"))
	//{
	//	return -1;
	//}
	//lightCubeShader->bind();

	Shader *ourShader = new Shader();
	if (!ourShader->loadShaderAsset("model_loading.vs", "model_loading.fs"))
	{
		return -1;
	}
	ourShader->bind();

	std::map<std::string, Shader*> shaderMap;
	//shaderMap.insert(std::pair<std::string, Shader*>("lightingShader", lightingShader));
	//shaderMap.insert(std::pair<std::string, Shader*>("lightCubeShader", lightCubeShader));
	shaderMap.insert(std::pair<std::string, Shader *>("ourShader", ourShader));
	shaderManager->addLoadShader(shaderMap);

	Model *ourModel = new Model();
	if (!ourModel->loadModelAsset("nanosuit.obj"))
	{
		return -1;
	}
	// 添加渲染贴图数据

	std::map<std::string, Model *> modelMap;
	modelMap.insert(std::pair<std::string, Model *>("ourModel", ourModel));
	mondelManager->addLoadModel(modelMap);

	std::vector<Model::MeshVertex> meshVertex = ourModel->getMeshVertex();
	RenderObject *object = new RenderObject();
	// 渲染模型
	// for(unsigned int i = 0; i < meshVertex.size(); i++)
	// {
	object->setRenderObject(meshVertex);
	render->addRenderObject(object);
	// }
	
	//float vertices[] = {
	//	// positions            // normals     // texture coords
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	//};

	//Vector3 positions[] = {
	//	Vector3(0.0f,  0.0f,  0.0f),
	//	Vector3(2.0f,  5.0f, -15.0f),
	//	Vector3(-1.5f, -2.2f, -2.5f),
	//	Vector3(-3.8f, -2.0f, -12.3f),
	//	Vector3(2.4f, -0.4f, -3.5f),
	//	Vector3(-1.7f,  3.0f, -7.5f),
	//	Vector3(1.3f, -2.0f, -2.5f),
	//	Vector3(1.5f,  2.0f, -2.5f),
	//	Vector3(1.5f,  0.2f, -1.5f),
	//	Vector3(-1.3f,  1.0f, -1.5f)
	//};

	//Vector3 pointLightPositions[] = {
	//	Vector3( 0.7f,  0.2f,  2.0f),
	//	Vector3( 2.3f, -3.3f, -4.0f),
	//	Vector3(-4.0f,  2.0f, -12.0f),
	//	Vector3( 0.0f,  0.0f, -3.0f)
	//};

	//RenderObject::VertexFormat vf;
	//vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	//vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	//vf.push_back({ 2, RenderObject::VertexAttr::ElementType::Float, false });

	//object->setRenderObject("cubeVAO", vf, vertices, 36, NULL, 0);
	//object->setRenderObject("lightCubeVAO", 8);
	//object->setPosition(10, positions);
	//object->setPointLightPositions(4, pointLightPositions);

	engine->run();
	engine->stopRun();

	return 0;
}