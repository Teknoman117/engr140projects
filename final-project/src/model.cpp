#include <project/model.hpp>
#include <project/lodepng.hpp>

#include <sstream>
#include <iostream>

namespace 
{
    class MaterialProgram
    {
        const Program& program;

        GLint attributeBoneWeights[4];
        GLint attributeTextures[2];
        GLint attributeNormal;
        GLint attributeVertex;
        GLint uniformBones[20];
        GLint uniformTextures[4];
        GLint uniformSkinned;
        GLint uniformTextured;
        GLint uniformColorAmbient;
        GLint uniformColorDiffuse;
        GLint uniformModelMatrix;
        GLint uniformSpecular;

    public:
        MaterialProgram(const Program& program);
        
        // Get the attribute for a bone weight
        const GLint AttributeBoneWeight(int idx) const;
        const GLint AttributeTexture(int idx) const;
        const GLint AttributeVertex();
        const GLint AttributeNormal();
        const GLint UniformBones(int idx) const;
        const GLint UniformTexture(Material::TextureType idx) const;
        const GLint UniformSkinned() const;
        const GLint UniformTextured() const;
        const GLint UniformColorAmbient() const;
        const GLint UniformColorDiffuse() const;
        const GLint UniformSpecularExponent() const;
        const GLint UniformModelMatrix() const;

        const Program& GetProgram() const
        {
            return program;
        }
    };

    MaterialProgram::MaterialProgram(const Program& program)
        : program(program)
    {
        attributeBoneWeights[0] = program.GetAttribute("boneWeight0");
        attributeBoneWeights[1] = program.GetAttribute("boneWeight1");
        attributeBoneWeights[2] = program.GetAttribute("boneWeight2");
        attributeBoneWeights[3] = program.GetAttribute("boneWeight3");
        attributeTextures[0]    = program.GetAttribute("vTex0");
        attributeTextures[1]    = program.GetAttribute("vTex1");
        attributeVertex         = program.GetAttribute("vVert");
        attributeNormal         = program.GetAttribute("vNorm");
        
        uniformTextures[Material::kTextureTypeDiffuse]  = program.GetUniform("diffuse");
        uniformTextures[Material::kTextureTypeNormal]   = program.GetUniform("normal");
        uniformTextures[Material::kTextureTypeSpecular] = program.GetUniform("specular");
        uniformTextures[Material::kTextureTypeBump]     = program.GetUniform("bump");
        
        for(int i = 0; i < 20; i++)
        {
            std::ostringstream b;
            b << "bones[" << i << "]" << std::ends;
            uniformBones[i] = program.GetUniform(b.str());
        }

        uniformSpecular     = program.GetUniform("SpecularExponent");
        uniformSkinned      = program.GetUniform("skinned");
        uniformTextured     = program.GetUniform("textured");
        uniformColorAmbient = program.GetUniform("AmbientColor");
        uniformColorDiffuse = program.GetUniform("DiffuseColor");
        uniformModelMatrix  = program.GetUniform("M");
        
        /*cout << "---- Material Program Init Log ----" << endl;
        cout << "Attribute Vertex: " << AttributeVertex() << endl;
        cout << "Attribute Normal: " << AttributeNormal() << endl;
        cout << "Attribute TexCd0: " << attributeTextures[0] << endl;
        cout << "Attribute TexCd1: " << attributeTextures[1] << endl;
        cout << "Attribute BoneW0: " << attributeBoneWeights[0] << endl;
        cout << "Attribute BoneW1: " << attributeBoneWeights[1] << endl;
        cout << "Attribute BoneW2: " << attributeBoneWeights[2] << endl;
        cout << "Attribute BoneW3: " << attributeBoneWeights[3] << endl;
        for(int i = 0; i < 20; i++)
        {
            cout << "Uniform Bone[" << i << "]: " << uniformBones[i] << endl;
        }
        cout << "-----------------------------------" << endl;*/
    }

    const GLint MaterialProgram::AttributeVertex() 
    {
        return attributeVertex;
    }
    const GLint MaterialProgram::AttributeNormal() 
    {
        return attributeNormal;
    }

    const GLint MaterialProgram::AttributeBoneWeight(int idx) const
    {
        return attributeBoneWeights[idx];
    }

    const GLint MaterialProgram::AttributeTexture(int idx) const
    {
        return attributeTextures[idx];
    }

    const GLint MaterialProgram::UniformBones(int idx) const
    {
        return uniformBones[idx];
    }

    const GLint MaterialProgram::UniformTexture(Material::TextureType idx) const
    {
        return uniformTextures[idx];
    }

    const GLint MaterialProgram::UniformSkinned() const
    {
        return uniformSkinned;
    }

    // Textured
    const GLint MaterialProgram::UniformTextured() const
    {
        return uniformTextured;
    }

    // Colors
    const GLint MaterialProgram::UniformColorAmbient() const
    {
        return uniformColorAmbient;
    }

    const GLint MaterialProgram::UniformColorDiffuse() const
    {
        return uniformColorDiffuse;
    }

    const GLint MaterialProgram::UniformModelMatrix() const
    {
        return uniformModelMatrix;
    }

    const GLint MaterialProgram::UniformSpecularExponent() const
    {
        return uniformSpecular;
    }
}

// Create an empty model for population manually
Model::Model(Resources &resources)
    : meshes(0, NULL), skeleton(new Node()), resources(resources), programWrapper(nullptr), uploaded(false)
{
    
}

// Create a model by deserializing it from Json
Model::Model(const Json::Value& root, const std::string& directory, Resources &resources)
    : meshes(0, NULL), skeleton(new Node()), resources(resources), programWrapper(nullptr), uploaded(false)
{
    loadAnimations(root);
    loadMaterials(root, directory);
    loadMeshes(root);
    loadSkeleton(root);
    loadParts(root);
}

Model::~Model()
{
    // Iterate through all meshes and free the data
    for(std::vector<Mesh *>::iterator it = meshes.begin(); it != meshes.end(); it++)
    {
        delete *it;
    }
    
    // Destroy all of the vertex buffers associated with meshes
    for(std::map<Mesh *, GLuint>::iterator it = buffers.begin(); it != buffers.end(); it++)
    {
        // If the contents are still a valid vertex buffer
        glDeleteBuffers(1, &it->second);
    }
    
    // Destroy all of the part render objects
    for(std::vector<Model::MeshPartRenderData *>::iterator it = renderables.begin(); it != renderables.end(); it++)
    {
        // Delete the index buffer
        glDeleteBuffers(1, &(*it)->indices);
		//Cleanup vertex arrays
        glDeleteVertexArrays(1, &(*it)->attributes);
        
        // Delete the object itself
        delete *it;
    }
    
    // Destroy all of the materials
    for(std::map<std::string, Material *>::iterator it = materials.begin(); it != materials.end(); it++)
    {
        delete it->second;
    }

    // Destroy all of the animations
    for(std::map<std::string, Animation *>::iterator it = animations.begin(); it != animations.end(); it++)
    {
        delete it->second;
    }
    
    // Destroy the skeletal nodes
    delete skeleton;
}

// Helper function to load animations form the model
void Model::loadAnimations(const Json::Value &root)
{
    // Load the animations
    const Json::Value &serializedAnimations = root["animations"];
    
    // If the model file contains animation entries
    if(serializedAnimations != Json::Value::null)
    {
        // Process the animations
        for(Json::Value::const_iterator it = serializedAnimations.begin(); it != serializedAnimations.end(); it++)
        {
            // Create a new animation object from the storage
            Animation *animation = new Animation(*it);
            
            // Store this animation
            animations[animation->Id()] = animation;
        }
    }
}

// Helper function to load the materials from the serialized Json blob
void Model::loadMaterials(const Json::Value &root, const std::string& directory)
{
    // Get the value for the mesh entry
    const Json::Value& mats = root["materials"];
    
    // If the model file contains mesh entries
    if(mats != Json::Value::null)
    {
        // Iterate through all the mesh entries
        for(Json::Value::const_iterator it = mats.begin(); it != mats.end(); it++)
        {
            // Load a material from the entry
            Material *material = new Material(*it);
            
            // Store this material for later usage
            materials[material->Id()] = material;
        }
    }
}

// Helper function to load the mesh data from the serialized Json blob
void Model::loadMeshes(const Json::Value& root)
{
    // Get the value for the mesh entry
    const Json::Value& meshes = root["meshes"];
    
    // If the model file contains mesh entries
    if(meshes != Json::Value::null)
    {
        // Iterate through all the mesh entries
        for(Json::Value::const_iterator it = meshes.begin(); it != meshes.end(); it++)
        {
            // Load a mesh from the json blob
            Mesh *mesh = new Mesh(*it);
            
            // Hold on to the mesh
            this->meshes.push_back(mesh);
        }
    }
}

// Helper function to load the node tree from the serialized json data
void Model::loadSkeleton(const Json::Value &root)
{
    // Get potential node entries
    const Json::Value& nodes = root["nodes"];
    
    // If the model contains node entries
    if(nodes != Json::Value::null)
    {
        // Create the node heirchy before search for mesh rendering information
        for(Json::Value::const_iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            // Create a new child node
            Node *child = new Node(*it, skeleton);
            
            // Add this child to our list
            skeleton->AddChild(child, false);
        }
    }
}

// Helper function to load the parts from the serialized json data
void Model::loadParts(const Json::Value &root)
{
    // Get potential node entries
    const Json::Value& nodes = root["nodes"];
    
    // If the model contains node entries
    if(nodes != Json::Value::null)
    {
        // Loop through all the nodes and check them for parts
        for(Json::Value::const_iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            loadPartsNodeSearch(*it);
        }
    }
}

// Recursive worker for the parts loader
void Model::loadPartsNodeSearch(const Json::Value &node)
{
    // Break out if this node is invalid
    if(node == Json::Value::null || !node.isObject())
    {
        return;
    }
    
    // Get the id for global inverse lookup
    std::string id = node["id"].asString();
    
    // If we find that we have a parts entry, explore it
    const Json::Value& parts = node["parts"];
    if(parts != Json::Value::null)
    {
        for(Json::Value::const_iterator it = parts.begin(); it != parts.end(); it++)
        {
            // Allocate a part to store data in
            Model::MeshPartRenderData *renderable = new Model::MeshPartRenderData();
            
            // Look up the mesh part this renderable uses
            std::string meshpartid = (*it)["meshpartid"].asString();
            for(std::vector<Mesh *>::iterator mIt = meshes.begin(); mIt != meshes.end(); mIt++)
            {
                // Iterate through mesh parts and put them in a map
                for(std::vector<Mesh::Part *>::iterator pIt = (*mIt)->Parts().begin(); pIt != (*mIt)->Parts().end(); pIt++)
                {
                    // If this is the part we are looking for
                    if((*pIt)->id == meshpartid)
                    {
                        // Store this mesh part
                        renderable->meshpart = *pIt;
                        
                        // get the fuck out (nested loop, can't break, need to force the iterators to the end)
                        mIt = meshes.end() - 1;
                        break;
                    }
                }
            }
            
            // Lookup the material this mesh uses
            std::string materialid = (*it)["materialid"].asString();
            renderable->material = materials[materialid];
            
            // Load the bones
            const Json::Value& bones = (*it)["bones"];
            if(bones != Json::Value::null)
            {
                for(Json::Value::const_iterator bIt = bones.begin(); bIt != bones.end(); bIt++)
                {
                    // Allocate a bone and deserialize
                    Model::Bone *bone = new Model::Bone(*bIt);
                    
                    // Store in the bones list
                    renderable->bones.push_back(bone);
                }
            }
            
            // Store the node this is attached to
            renderable->id = id;
            
            // Store this renderable data in our renderable list
            renderables.push_back(renderable);
        }
    }
    
    // Explore children if we have any
    const Json::Value& childs = node["children"];
    if(childs != Json::Value::null)
    {
        for(Json::Value::const_iterator it = childs.begin(); it != childs.end(); it++)
        {
            // Explore the children
            loadPartsNodeSearch(*it);
        }
    }
}

// Upload the data to the GPU
void Model::Upload()
{
    // If we are uploaded, get outta here
    if(uploaded)
    {
        return;
    }
    
    // Upload the mesh data
    for(std::vector<Mesh *>::iterator it = meshes.begin(); it != meshes.end(); it++)
    {
        // Create a vertex buffer for this mesh
        GLuint vertexBufferObject = 0;
        glGenBuffers(1, &vertexBufferObject);
        
        // Upload the data to the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, (*it)->Data().size() * sizeof(GLfloat), (*it)->Data().data(), GL_STATIC_DRAW);
        
        // Post this mesh to the buffers list
        buffers[(*it)] = vertexBufferObject;
    }
    
    // Upload the mesh part data
    for(std::vector<MeshPartRenderData *>::iterator it = renderables.begin(); it != renderables.end(); it++)
    {
		auto & renderable = *it;
        // Create the vertex array object for this renderable
        glGenVertexArrays(1, &renderable->attributes);

        // Create the vertex buffer for the indices
        glGenBuffers(1, &renderable->indices);
        
        // Upload the index data to the gpu
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderable->meshpart->indices.size() * sizeof(GLushort), renderable->meshpart->indices.data(), GL_STATIC_DRAW);
    }
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	//glBindVertexArray( 0 );
    
    // We are now uploaded
    uploaded = true;
}

// Update function
void Model::Update(double delta, double now)
{
    // If we need to upload the mesh data, do so now
    if(!uploaded)
    {
        std::cout << "uploading" << std::endl;
        Upload();
        std::cout << "done" << std::endl;
    }
    
    // Other shit
}

// Render method - forward the base skeleton to the model
void Model::Draw(const std::string& program, const glm::mat4& offset)
{
    Draw(program, *skeleton, offset);
}

// Render method - provide a skeleton to the model
void Model::Draw(const std::string& program, const Node& _skeleton, const glm::mat4& offset)
{
    std::cout << "drawing" << std::endl;
    // If we are not uploaded, throw an exception
    if(!uploaded)
    {
        //throw new std::runtime_error("void Model::Draw(const GLMeshProgram *program) - Attempted to draw a model not uploaded to the GPU");
        return;
    }

    const Program& p = resources.GetProgram(program);
    bool programChanged = false;
    if(!programWrapper || p.GetHandle() != GetProgram<MaterialProgram>()->GetProgram().GetHandle())
    {
        programChanged = true;
        programWrapper = new MaterialProgram(p);
    }
    
    // Use standard blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Iterate through all the renderables
    for(std::vector<Model::MeshPartRenderData *>::iterator renderable = renderables.begin(); renderable != renderables.end(); renderable++)
    {
        // Find the node this thing is attached to
        const Node *node = _skeleton.FindNode((*renderable)->id);
        
        std::cout << "mat" << std::endl;

        // Calculate the global inverse transform of the mesh (for internal scaling purposes)
        glm::mat4 globalInverseTransform = glm::inverse(node->TransformMatrix());
        glm::mat4 modelMatrix = offset * node->TransformMatrix();

        std::cout << "get prog" << std::endl;

        glUniformMatrix4fv(GetProgram<MaterialProgram>()->UniformModelMatrix(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        
        std::cout << "bind vert " << std::endl;
        // Bind the vertex array object
        glBindVertexArray((*renderable)->attributes);
        
        // Update the program attributes if the shader has changed
        if(programChanged)
        {
            std::cout << "prog changed" << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, buffers[(*renderable)->meshpart->mesh]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*renderable)->indices);
            
            // Iterate through the vertex attributes (dynamic shader parameters, as model vertex attributes vary)
            for(VertexAttributes::const_iterator attribute = (*renderable)->meshpart->mesh->Attributes().begin(); attribute != (*renderable)->meshpart->mesh->Attributes().end(); attribute++)
            {
                // Convert the attribute key into the shader attribute (in the future these should be equivalent - via layout=x)
                GLuint programAttribute = INT_MAX;
                switch (attribute->attribute)
                {
                    case VertexAttributes::kAttributeVertex:
                        programAttribute = GetProgram<MaterialProgram>()->AttributeVertex();
                        break;
                        
                    case VertexAttributes::kAttributeNormal:
                        programAttribute = GetProgram<MaterialProgram>()->AttributeNormal();
                        break;
                        
                    case VertexAttributes::kAttributeTextureCoordinate:
                        // Return the right texture coordinate mapping
                        programAttribute = GetProgram<MaterialProgram>()->AttributeTexture(attribute->index);
                        break;
                        
                    case VertexAttributes::kAttributeBoneWeight:
                        // Return the correct bone weight mapping
                        programAttribute = GetProgram<MaterialProgram>()->AttributeBoneWeight(attribute->index);
                        break;
                        
                    default:
                        // Skip unknown attributes
                        continue;
                        //throw new std::runtime_error("void Model::Draw(const GLMeshProgram *program) - Unrecognized shader attribute");
                        //break;
                }
                
                // If something goes wrong, skip it
                if(programAttribute == -1) continue;
                
                // Enable the vertex array object entry for this attribute
                glEnableVertexAttribArray(programAttribute);

                // Update the vertex attribute pointer
                glVertexAttribPointer(programAttribute, (GLsizei) VertexAttributes::AttributeSize(attribute->attribute), GL_FLOAT, GL_FALSE, (GLsizei) (*renderable)->meshpart->mesh->Attributes().AttributeFrameSize() * sizeof(GLfloat), (GLvoid *)(attribute->offset * sizeof(GLfloat)));
            }
            
        }

        // Bind the material's textures to some texturing units
        /*for(Material::texture_store::const_iterator tIt = (*renderable)->material->Textures().begin(); tIt != (*renderable)->material->Textures().end(); tIt++)
        {
            glUniform1i(GetProgram<MaterialProgram>()->UniformTexture(tIt->first), tIt->first);
            resources.GetTexture(tIt->second).Bind(GL_TEXTURE0 + tIt->first);
        }
        
        // Set all the bones according to the skeleton
        GLint boneIdx = 0;
        for(std::vector<Model::Bone *>::iterator bone = (*renderable)->bones.begin(); bone != (*renderable)->bones.end(); bone++, boneIdx++)
        {
            // Get the node which cooresponds to our bone
            const Node *node = _skeleton.FindNode((*bone)->id);
            
            // Calculate the final bone transform
            glm::mat4 finalTransform = globalInverseTransform * node->TransformMatrix() * (*bone)->inverseTransformMatrix;
            
            // Upload the bone to the shader
            glUniformMatrix4fv(GetProgram<MaterialProgram>()->UniformBones(boneIdx), 1, GL_FALSE, glm::value_ptr(finalTransform));
        }
        
        // Pass the bone count through (well, anything more than zero means skinned)
        glUniform1i(GetProgram<MaterialProgram>()->UniformSkinned(), (GLint)(*renderable)->bones.size());
        
        // Pass the texture count through (anything more than zero means skinned)
        glUniform1i(GetProgram<MaterialProgram>()->UniformTextured(), (GLint)(*renderable)->material->Textures().size());
        
        // Set the color
        glm::vec3 ambient = (*renderable)->material->ColorAmbient();
        glm::vec3 diffuse = (*renderable)->material->ColorDiffuse();
        glUniform3f(GetProgram<MaterialProgram>()->UniformColorAmbient(), ambient.r, ambient.g, ambient.b);
        glUniform3f(GetProgram<MaterialProgram>()->UniformColorDiffuse(), diffuse.r, diffuse.g, diffuse.b);
        
        // Should we use blending
        if((*renderable)->material->Opaque())
        {
            glDisable(GL_BLEND);
        } else
        {
            glEnable(GL_BLEND);
        }
        
        // Finally, we can draw the god damn mesh
        glDrawElements(GL_TRIANGLES, (GLsizei) (*renderable)->meshpart->indices.size(), GL_UNSIGNED_SHORT, NULL);*/
    }
    
    // Don't fuck the VAOs
    glBindVertexArray(0);
}

const Node* Model::Skeleton() const
{
    return skeleton;
}

const std::map<std::string, Animation *>& Model::Animations() const
{
    return animations;
}

// Standard constructor (initialize everything)
Model::Bone::Bone()
    : id(""), transform(Transform())
{
    
}

// Deserialization constructor
Model::Bone::Bone(const Json::Value& value)
{
    // We first need to validate that this a Json object
    if(!value.isObject())
    {
        throw std::runtime_error("Model::Bone::Bone(const Json::Value& value) - value must be a object");
    }
    
    // Load the name of the material
    id = value["node"].asString();
    
    // Load the transform
    transform = Transform(value);
    
    // Create an inverse transform matrix
    inverseTransformMatrix = glm::inverse(transform.TransformMatrix());
}

// Standard constructor
Model::MeshPartRenderData::MeshPartRenderData()
    : meshpart(NULL), material(NULL), bones(0, NULL), attributes(0), indices(0)
{
    
}

Model::MeshPartRenderData::~MeshPartRenderData()
{
    // Iterate through mesh parts and put them in a map
    for(std::vector<Model::Bone *>::iterator bIt = bones.begin(); bIt != bones.end(); bIt++)
    {
        delete *bIt;
    }
}


// Load a model from a Json file
Model* Model::LoadFromJsonFile(const std::string &directory, const std::string &name, const Transform& offset, Resources& resources)
{
    // Compute filename
    std::string filename = directory + "/" + name;
    
    // Load the compressed data from disk
    std::vector<unsigned char> buffer;
    lodepng::load_file(buffer, filename);
    
    // If the loading of the file failed, error out
    if(buffer.size() == 0)
    {
        throw new std::runtime_error("Model* Model::LoadFromJsonFile() - Failed to load: " + filename);
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
        throw new std::runtime_error("Model* Model::LoadFromJsonFile() - Parsing failed for: " + filename);
    }
    
    // Create the model object
    Model *model = new Model(root, directory, resources);
    
    // Adjust the root transform of the skeleton to coorespond with the offset
    model->skeleton->LocalTransform() = offset;
    model->skeleton->Recalculate();
    
    // Return an allocated model
    return model;
}

// Load a model from a compressed Json file
Model* Model::LoadFromCompressedJsonFile(const std::string &directory, const std::string &name, const Transform& offset, Resources& resources)
{
    // Compute filename
    std::string filename = resources.GetPath() + "/" + directory + "/" + name;
    
    // Load the compressed data from disk
    std::vector<unsigned char> compressedData;
    lodepng::load_file(compressedData, filename);
    
    // If the loading of the file failed, error out
    if(compressedData.size() == 0)
    {
        throw new std::runtime_error("Model* Model::LoadFromCompressedJsonFile() - Failed to load: " + filename);
    }
    
    // Decompress this data (since we are dealing with large file, use the least amount of copies as possible
    unsigned char *buffer = 0;
    size_t buffersize = 0;
    unsigned error = lodepng_zlib_decompress(&buffer, &buffersize, compressedData.data(), compressedData.size(), &lodepng_default_decompress_settings);
    
    // If the decompression was not successful, error out
    if(!buffer || error)
    {
        throw new std::runtime_error("Model* Model::LoadFromCompressedJsonFile() - decompression of mesh failed");
    }

    // Calculate buffer pointers for json decode
    const char *pBegin = (const char *) buffer;
    const char *pEnd = (const char *) (buffer + buffersize);
    
    // Create a json parser for this file
    Json::Reader reader;
    Json::Value  root;
    
    // Read the model file
    if(!reader.parse(pBegin, pEnd, root))
    {
        throw new std::runtime_error("Model* Model::LoadFromCompressedJsonFile() - Parsing failed for: " + filename);
    }
    
    // Create the model object
    Model *model = new Model(root, directory, resources);
    
    // Adjust the root transform of the skeleton to coorespond with the offset
    model->skeleton->LocalTransform() = offset;
    model->skeleton->Recalculate();
    
    // Release the zlib decompression buffer
    free(buffer);
    
    // Return an allocated model
    return model;
}
