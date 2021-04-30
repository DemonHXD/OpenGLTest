#include "file_system.h"

#include <io.h>
#include <iomanip>
#include <direct.h>

#include "../common/json_load.h"

FileSystem *FileSystem::pInstance = nullptr;

FileSystem *FileSystem::getInstance()
{
    if (nullptr == pInstance)
    {
        pInstance = new FileSystem();
    }
    return pInstance;
}

void FileSystem::init()
{
    preLoadAllAsset();
    JsonLoad::getInstance()->initJson(getFilesBySuffix(".json"));
}

/*
    获取路径下的所有文件
*/
void FileSystem::getFiles(std::string path, std::vector<std::string> &files)
{
    //文件句柄
    long hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

/*
    预加载所有资源，把资源名+路径存储起来
*/
void FileSystem::preLoadAllAsset()
{
    std::vector<std::string> files;
    std::string assetPath = getEnginePath() + std::string("asset");
    getFiles(assetPath, files);

    int size = files.size();
    for (int i = 0; i < size; i++)
    {
        std::string assetPath = files[i];
        int idx = assetPath.rfind('\\');
        std::string assetName = assetPath.substr(idx + 1, assetPath.size());
        m_assets.insert(std::pair<std::string, std::string>(assetName, assetPath));
    }
}

/*
	获取引擎所在目录
*/
std::string FileSystem::getEnginePath() const
{
    int index = 0;
    char buffer[_MAX_PATH];
    _getcwd(buffer, _MAX_PATH);
    while (buffer[index] != '\0')
    {
        index++;
    }
    for (; index >= 0; index--)
    {
        if (buffer[index] == '\\')
        {
            break;
        }
        buffer[index] = '\0';
    }
    return buffer;
}

/*
    通过后缀名获取所有资源
*/
std::vector<std::string> FileSystem::getFilesBySuffix(std::string suffixName)
{
    std::vector<std::string> files;

    std::string assetPath = getEnginePath() + std::string("asset");
    //获取该路径下的所有文件
    getFiles(assetPath, files);
    auto iter = files.begin();
    while (iter != files.end())
    {
        std::string assetPath = *iter;
        int result = assetPath.find(suffixName);
        if (result == -1)
        {
            iter = files.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    return files;
}

/*
    通过资源名获取资源路径(资源名不能重复，否则以第一个匹配到的为准)
*/
const std::string &FileSystem::getAssetPathByName(std::string assetName) const
{
    return m_assets.at(assetName);
}
