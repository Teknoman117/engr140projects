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

#ifndef __MODEL_INSTANCE_H__
#define __MODEL_INSTANCE_H__

#include <project/common.hpp>
#include <project/animationclip.hpp>
#include <project/model.hpp>

class ModelInstance
{
    /** Model we are providing an instance of */
    std::shared_ptr<Model>    model;
    
    /** The node representing this model's position in space */
    Node                     *node;
    
    /** SUPER DUPER TESTING MODE SHIT */
    AnimationSource          *nullAnimation;
    AnimationSource          *animation;
public:
    /**
     * Create a model instance for a model
     * @param _model Model to provide an instance of
     */
    ModelInstance(Model *_model);
    
    /** 
     * Copy constructor
     * @param instance ModelInstance to duplicate (does not duplicate model)
     */
    ModelInstance(const ModelInstance& instance);
    
    /**
     * Update the state of this model instance
     * @param delta time since last frame in seconds
     * @param now the current time
     */
    void Update(double delta, double now);
    
	/*
	* Modify the animation directly, cause we be crazy man
	*/
	AnimationSource* Animation();

    /**
     * Draw this model instance with a particular shader
     * @param program the shader to draw this model with
     */
    void Draw(const std::string& program);
    
    /**
     * Get a pointer to the node of the model
     * @return pointer to the model
     */
    Node* GetNode();
    
    /**
     * Get a const pointer to the node of the model
     * @return const pointer to the model
     */
    const Node* GetNode() const;
    
    /**
     * Get a pointer to the model this instance uses
     * @return pointer to model object
     */
    const Model* GetModel() const;
    
    /**
     *
     */
    glm::mat4 GetTransformOfSkeletalNode(std::string name);
    
    /**
     * Static method to construct a model instance from a manifest file
     * @param value The Json serialized value to build this model instance from
     * @return Pointer to a model instance encapsulating the entry
     */
    static ModelInstance* LoadManifestEntry(const Json::Value& model, Resources& resources);
    
    /** Test stuff **/
    bool PlayAnimation(const std::string name, double now);
    void PlayCustomAnimation(AnimationSource *animationSource);
    void PlayNullAnimation();
    const Node* Skeleton() const;
};

#endif
