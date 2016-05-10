#ifndef __MODEL_GROUP_HPP__
#define __MODEL_GROUP_HPP__

#include <project/common.hpp>
#include <project/resources.hpp>
#include <project/model.hpp>
#include <project/modelinstance.hpp>
 
#include <mutex>

class ModelGroup
{
public:
    // Iterator types
    typedef std::map<std::string, ModelInstance *>::const_iterator const_iterator;
    typedef std::map<std::string, ModelInstance *>::size_type size_type;
    
private:
	//Protect the map while adding to the model group
	std::mutex modelGroupMutex;

    // The models in the model group
    std::map<std::string, ModelInstance *> models;
    
    // Texture cache for the models
    Resources&  resources;

    void Load(const Json::Value& root);

public:
    // Create an empty model group
    ModelGroup(Resources& resources);
    ModelGroup(const Json::Value& manifest, Resources& resources);
    ModelGroup(const std::string& manifestPath, Resources& resources);

    // Create a new model instance from a model in the group
    ModelInstance* NewInstance(const std::string modelName);
    
    // Get iterators
    const_iterator begin() const;
    const_iterator end()   const;
    size_type      size()  const;
};

#endif
