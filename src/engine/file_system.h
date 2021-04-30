#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include <string>
#include <vector>
#include <map>

class FileSystem
{
public:
    static FileSystem *getInstance();
    void init();

    std::string getEnginePath() const;
    std::vector<std::string> getFilesBySuffix(std::string suffixName);
    const std::string &getAssetPathByName(std::string assetName) const;

private:
    void preLoadAllAsset();
    void getFiles(std::string path, std::vector<std::string> &files);

    FileSystem(){};
    static FileSystem *pInstance;

    std::map<std::string, std::string> m_assets;
};

#endif