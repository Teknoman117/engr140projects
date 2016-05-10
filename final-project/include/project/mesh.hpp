#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <project/common.hpp>
#include <project/vertexattributes.hpp>

// The mesh is an object which
class Mesh
{
public:
    // Subclass which handles subcomponents of the mesh (they are just index buffers though)
    struct Part
    {
        // Owning mesh (for buffer data lookup)
        Mesh *mesh;
        
        // Mesh parts are assigned names
        std::string id;
        
        // Mesh parts only consist of an index buffer
        std::vector<GLushort> indices;
        
        // Base initialization just nullifies everything
        Part();
        
        // Deserialize a mesh part from Json
        Part(const Json::Value& value);
    };
    
private:
    // For performance reasons (at least according to the iOS profiler), mesh data is interleaved
    std::vector<GLfloat> data;
    
    // Indices into the interleaven vertex data
    std::vector<GLushort>  indices;
    
    // Offset calculation for interleaven data
    VertexAttributes     attributes;
    
    // The individual components of the mesh (separated by material, vertex count, etc.)
    std::vector<Part *>  parts;
    
public:
    // Create an empty mesh for population manually
    Mesh();
    
    // Create a mesh by deserializing it from Json
    Mesh(const Json::Value& value);
    
    // Standard deconstructor - frees all heap allocated data
    ~Mesh();
    
    // Get references to the mesh data
    std::vector<GLfloat>& Data();
    std::vector<GLushort>&  Indices();
    VertexAttributes&     Attributes();
    std::vector<Part *>&  Parts();
};

#endif
