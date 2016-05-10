/*
 *  Copyright 2016 Nathaniel Lewis
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

#include "ModelInstance.h"
#include "AnimationTransition.h"
#include "OS.h"

//#define __MODELINSTANCE_PRINT_LOGS__

/**
 * Create a model instance for a model
 * @param _model Model to provide an instance of
 */
ModelInstance::ModelInstance(Model *_model)
    : model(_model), node(new Node())
{
    nullAnimation = new AnimationSource();
    nullAnimation->Bind(model->Skeleton());
    animation = nullAnimation;
}

/**
 * Copy constructor
 * @param instance ModelInstance to duplicate (does not duplicate model)
 */
ModelInstance::ModelInstance(const ModelInstance& instance)
    : model(instance.model), node(new Node())
{
    node->LocalTransform() = instance.GetNode()->LocalTransform();
    nullAnimation = new AnimationSource();
    nullAnimation->Bind(model->Skeleton());
    animation = nullAnimation;
}

/**
 * Update the state of this model instance
 * @param delta time since last frame in seconds
 * @param now the current time
 */
void ModelInstance::Update(double delta, double now)
{
    // Make sure our model is uploaded
    model->Update(delta, now);
    
    // Update the animation test
    animation->Update(delta, now);
}

/*
* Modify the animation directly, cause we be crazy man
*/
AnimationSource* ModelInstance::Animation()
{
	return animation;
}

/**
 * Draw this model instance with a particular shader
 * @param program the shader to draw this model with
 */
void ModelInstance::Draw(MaterialProgram *program)
{
    // Push the model transform onto the program's matrix stack
    program->Model.PushMatrix();
    program->Model.SetMatrix(node->TransformMatrix());

    animation->Skeleton()->Recalculate();
    model->Draw(program, *(animation->Skeleton()));
    
    // Remove the translation
    program->Model.PopMatrix();
}

/**
 * Get a pointer to the node of the model
 * @return pointer to the model
 */
Node* ModelInstance::GetNode()
{
    return node;
}

/**
 * Get a const pointer to the node of the model
 * @return const pointer to the model
 */
const Node* ModelInstance::GetNode() const
{
    return node;
}

/**
 * Get a const pointer to the model object of the instance
 * @return const pointer to model
 */
const Model* ModelInstance::GetModel() const
{
    return model.get();
}

/**
 *
 */
mat4 ModelInstance::GetTransformOfSkeletalNode(std::string name)
{
    // Get the current bone
    Node *animationNode = animation->Skeleton()->FindNode(name);
    if(!animationNode)
        return node->TransformMatrix();
    
    // Return a composed transformation for the node
    return node->TransformMatrix() * animationNode->TransformMatrix();
}

/**
 * Static method to construct a model instance from a manifest file
 * @param value The Json serialized value to build this model instance from
 * @return Pointer to a model instance encapsulating the entry
 */
ModelInstance* ModelInstance::LoadManifestEntry(const Json::Value& model, TextureCache& textureCache)
{
    // Get the model information
    std::string name = (model)["name"].asString();
    std::string path = (model)["path"].asString();
    std::string directory = (model)["directory"].asString();
    bool compressed = (model)["compressed"].asBool();
    
    // The offset transform
    Transform transform;
    
    // Search for an offset transform
    const Json::Value& offset = (model)["offset"];
    
    // If an offset was specified in the manifest
    if(offset != Json::Value::null && offset.isObject())
    {
        transform = Transform(offset);
    }
    
    // Load the model from json or compressed json
    ModelInstance *instance = NULL;
    if(compressed)
    {
        instance = new ModelInstance(Model::LoadFromCompressedJsonFile(directory, path, transform, textureCache));
    } else
    {
        instance = new ModelInstance(Model::LoadFromJsonFile(directory, path, transform, textureCache));
    }
    
    // Search for an animation controller
    /*const Json::Value& controller = (model)["controller"];
    
    // If an animation controller was defined, load it
    if(controller != Json::Value::null && controller.isObject())
    {
        // Forcibly load the controller
        delete instance->controller;
        instance->controller = new AnimationController(controller, instance->model.get());
    }*/
    
#if (defined __MODELINSTANCE_PRINT_LOGS__)
    // Print out the animations
    for(Model::animation_const_iterator it = instance->model->Animations().begin(); it != instance->model->Animations().end(); it++)
    {
        cout << "Animation: " << it->first << " is " << it->second->Length() << " seconds" << endl;
    }
#endif
    
    // Return the created instance
    return instance;
}

// TEST TEST TEST
bool ModelInstance::PlayAnimation(const std::string name)
{
    // Get an iterator to the animation we want to play
    Model::animation_const_iterator anim = model->Animations().find(name);
    
    // If the iterator is invalid, this failed, so escape
    if(anim == model->Animations().end())
    {
        return false;
    }
    
    // Create the animation clip for the new animation
    AnimationClip *animationClip = new AnimationClip(anim->second);
    animationClip->Bind(model->Skeleton());
    animationClip->Play(true, OS::Now());
    
    AnimationTransition *transition = new AnimationTransition;
    transition->Bind(model->Skeleton());
    transition->Start(animation, animationClip, 0.25, [this] (AnimationTransition *source, AnimationSource *from, AnimationSource *to)
    {
        if(source == this->animation)
        {
            this->animation = to;
            delete source;
        }
    });
    animation = transition;
    
    
    // Return success
    return true;
}

void ModelInstance::PlayCustomAnimation(AnimationSource *animationSource)
{
    // Create the animation clip for the new animation
    AnimationTransition *transition = new AnimationTransition;
    transition->Bind(model->Skeleton());
    transition->Start(animation, animationSource, 0.25, [this] (AnimationTransition *source, AnimationSource *from, AnimationSource *to)
    {
        if(source == this->animation)
        {
            this->animation = to;
            delete source;
        }
    });
    animation = transition;
}

void ModelInstance::PlayNullAnimation()
{
    // Create the animation clip for the new animation
    AnimationTransition *transition = new AnimationTransition;
    transition->Bind(model->Skeleton());
    transition->Start(animation, nullAnimation, 0.25, [this] (AnimationTransition *source, AnimationSource *from, AnimationSource *to)
    {
        if(source == this->animation)
        {
            this->animation = to;
            delete source;
        }
    });
    animation = transition;
}

const Node* ModelInstance::Skeleton() const
{
    return animation->Skeleton();
}
