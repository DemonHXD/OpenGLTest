#ifndef _JSON_LOAD_H_
#define _JSON_LOAD_H_
#include <string>
#include <map>
#include <vector>
#include <cassert>
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
        XPACK(O(ambient, diffuse, specular, constant, linear, quadratic));
    };

public:
    static JsonLoad *getInstance();

    void initJson(std::vector<std::string> files);

	template<typename T> void getJsonData(T &t, std::string jsonName)
    {
        std::string assetPath = m_json_paths.at(jsonName);
        xpack::json::decode(getStringFromFile(assetPath), t);
	}

private:
    JsonLoad(){};
    static JsonLoad *pInstance;
    std::string getStringFromFile(std::string jsonFilePath);

    std::map<std::string, std::string> m_json_paths;
};
#endif // !_JSON_LOAD_H_