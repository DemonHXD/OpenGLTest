#include "model_manager.h"
#include <iostream>
#include <assert.h>
ModelManager *Singleton<ModelManager>::singleton = nullptr;

void ModelManager::addLoadModel(std::map<std::string, Model*> models)
{
    std::map<std::string, Model*>::iterator it;
    for(it = models.begin(); it != models.end(); it++)
    {
        m_models.insert(std::pair<std::string, Model*>(it->first, it->second));
    }
}