#ifndef _JSON_LOAD_H_
#define _JSON_LOAD_H_
#include <string>
#include <map>
#include <vector>
#include "math.h"
#include "xpack/json.h"


XPACK_OUT(Vector3, O(x, y, z));

class JsonLoad
{
public:
    struct DirLightData
    {
        Vector3 direction;
        Vector3 ambient;
        Vector3 diffuse;
        Vector3 specular;
        bool isInit = false;
        XPACK(O(direction, ambient, diffuse, specular));
    };

	struct PointLightsData
	{
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		float constant;
		float linear;
		float quadratic;
		bool isInit = false;
		XPACK(O(ambient, diffuse, specular, constant, linear, quadratic));
	};

public:
    static JsonLoad *getInstance();

    void initJson(std::vector<std::string> files);

    DirLightData getDirLightData();
	PointLightsData getPointLightsData();
	PointLightsData getSpotLightData();

private:
    JsonLoad(){};
    static JsonLoad *pInstance;
    std::string getStringFromFile(std::string jsonFilePath);

    DirLightData m_dirLight_data;
	PointLightsData m_pointLights_data;
	PointLightsData m_spotLight_data;
    std::map<std::string, std::string> m_json_paths;
};
#endif // !_JSON_LOAD_H_