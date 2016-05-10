/*
 *  Copyright 2014 Nathaniel Lewis
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <project/lodepng.hpp>
#include <project/semaphore.hpp>
#include <project/modelgroup.hpp>
#include <thread>
#include <iostream>


using namespace std::chrono;

// Cdte an empty model group
ModelGroup::ModelGroup(Resources& resources)
    : resources(resources)
{
    
}

// Create a model group and load the models from the manifest
ModelGroup::ModelGroup(const Json::Value& root, Resources& resources)
    : resources(resources)
{
    Load(root);
}

// Create a model group and load the models from the manifest
ModelGroup::ModelGroup(const std::string& manifestPath, Resources& resources)
    : resources(resources)
{
    // Load the compressed data from disk
    std::vector<unsigned char> buffer;
    lodepng::load_file(buffer, manifestPath);
    
    // If the loading of the file failed, error out
    if(buffer.size() == 0)
    {
        throw new std::runtime_error("Model::Model(const string& manifestPath, Resources& resources) - Failed to load: " + manifestPath);
    }
    
    // Calculate buffer pointers for json decode
    const char *pBegin = (const char *) buffer.data();
    const char *pEnd = (const char *) (buffer.data() + buffer.size());
    
    // Create a json parser for this file
    Json::Reader reader;
    Json::Value  root;
    
    // Read the model file
    if(!reader.parse(pBegin, pEnd, root))
    {
        throw new std::runtime_error("Model::Model(const string& manifestPath, Resources& resources) - Parsing failed for: " + manifestPath);
    }
    
    Load(root);
}

void ModelGroup::Load(const Json::Value& root)
{
    // The root must be an array
    if(root == Json::Value::null || !root.isArray())
    {
        return;
    }
    
    // Get start time to measure the load time of the models
    high_resolution_clock::time_point start = high_resolution_clock::now();

    // Get an iterator to the current Json value
    Json::Value::const_iterator modelIterator = root.begin();

    // Create a mutex to lock the critical sections of the model loader worker threads
    std::mutex modelIteratorMutex;
    semaphore  completedThreads;

    static const int workerThreads = std::thread::hardware_concurrency();

    // Launch four worker threads
    for (int i = 0; i < workerThreads; i++)
    {
        std::thread t([&] ()
        {
            while (1) 
            {
                //Check if all models are loaded - breakout if so
                std::unique_lock<std::mutex> lock(modelIteratorMutex);
                if (modelIterator == root.end())
                {
                    completedThreads.notify();
                    return;
                }
                
                // Otherwise, get a reference to the current model to load
                const Json::Value& model = *modelIterator;
                modelIterator++;
                lock.unlock();

                // Get the name of the model
                std::string name = (model)["name"].asString();
                
                // Create a ModelInstance from this entry
                high_resolution_clock::time_point s = high_resolution_clock::now();
                ModelInstance *instance = ModelInstance::LoadManifestEntry(model, resources);
                std::cout << "[INFO] Loaded: " << name << " (in " << duration_cast<duration<double> >(high_resolution_clock::now() - s).count() << " seconds)" << std::endl;
                
                // Insert this into the model group
                modelGroupMutex.lock();
                models[name] = instance;
                modelGroupMutex.unlock();
            }
        });
        t.detach();
    }
    
    //Decrease four times, once for each thread
    for (int i = 0; i < workerThreads; i++)
    {
        completedThreads.wait();
    }

    std::cout << "[INFO] Loaded Models (in " << duration_cast<duration<double>>(high_resolution_clock::now() - start).count() << " seconds)" << std::endl;
}

// Create a new model instance from a model in the group
ModelInstance* ModelGroup::NewInstance(const std::string modelName)
{
    // Lookup the model name
    std::map<std::string, ModelInstance *>::iterator model = models.find(modelName);
    
    // If the model loaded successfully
    if(model != models.end())
    {
        ModelInstance *instance = new ModelInstance(*(model->second));
        return instance;
    }
    
    // Otherwise fail out
    return NULL;
}

// Get iterators
ModelGroup::const_iterator ModelGroup::begin() const
{
    return models.begin();
}

ModelGroup::const_iterator ModelGroup::end() const
{
    return models.begin();
}

ModelGroup::size_type ModelGroup::size() const
{
    return models.size();
}
