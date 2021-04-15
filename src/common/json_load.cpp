#include "json_load.h"
#include "../engine/engine.h"
#include <fstream>
#include <sstream>
#include <iostream>
JsonLoad *JsonLoad::pInstance = nullptr;

JsonLoad *JsonLoad::getInstance()
{
    if (nullptr == pInstance)
    {
        pInstance = new JsonLoad();
    }
    return pInstance;
}

void JsonLoad::initJson(std::vector<std::string> files)
{
    for (std::string assetPath : files)
    {
        int idx = assetPath.rfind('\\');
        std::string assetName = assetPath.substr(idx + 1, assetPath.size());
        assetName = assetName.substr(0, assetName.rfind("."));
        m_json_paths.insert(std::pair<std::string, std::string>(assetName, assetPath));
    }
}

std::string JsonLoad::getStringFromFile(std::string jsonFilePath)
{
    // 1.从文件路径中获取顶点/片段着色器
    std::string jsonCode;
    std::ifstream jsonFile;
    // 保证ifstream对象可以抛出异常
    jsonFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //打开文件
        jsonFile.open(jsonFilePath);
        std::stringstream jsonStream;
        // 读取文件的缓冲内容到数据流中
        jsonStream << jsonFile.rdbuf();
        // 关闭文件处理器
        jsonFile.close();
        // 转换数据流到string
        jsonCode = jsonStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "json read fail" << std::endl;
        return false;
    }
    return jsonCode;
}

JsonLoad::DirLightData JsonLoad::getDirLightData()
{
    if (!m_dirLight_data.isInit)
    {
        std::string assetPath = m_json_paths.at("dirLight");
        xpack::json::decode(getStringFromFile(assetPath), m_dirLight_data);
        m_dirLight_data.isInit = true;
    }
    return m_dirLight_data;
}

JsonLoad::PointLightsData JsonLoad::getPointLightsData()
{
    if (!m_pointLights_data.isInit)
    {
        std::string assetPath = m_json_paths.at("pointLights");
        xpack::json::decode(getStringFromFile(assetPath), m_pointLights_data);
        m_pointLights_data.isInit = true;
    }
    return m_pointLights_data;
}

JsonLoad::PointLightsData JsonLoad::getSpotLightData()
{
    if (!m_spotLight_data.isInit)
    {
        std::string assetPath = m_json_paths.at("spotLight");
        xpack::json::decode(getStringFromFile(assetPath), m_spotLight_data);
        m_spotLight_data.isInit = true;
    }
    return m_spotLight_data;
}