#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

#include "../common/singleton.h"
#include <map>

class Model;
class ModelManager : public Singleton<ModelManager>
{
public:
    void addLoadModel(std::map<std::string, Model*> models);

    std::map<std::string, Model*> getModels() const
    {
        return m_models;
    }
private:
    std::map<std::string, Model*> m_models;
};

#endif