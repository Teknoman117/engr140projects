#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <project/common.hpp>
#include <project/transform.hpp>
#include <json/json.h>


// Class to wrap rotation, translation, and scale transformations
class Node
{
public:
    // Flat tree map types
    typedef std::map<std::string, Node *> flattreemap;
    typedef std::map<std::string, const Node *> const_flattreemap;
    
private:
    // Name of the node
    std::string          id;
    
    // The global transformation matrix
    glm::mat4            globalTransformMatrix;
    
    // The transform of this node
    Transform            transform;
    
    // This node's children
    std::vector<Node *>  children;
    
    // This node's parent
    Node                *parent;
    
public:
    // Construct an empty node
    Node();
    
    // Construct a node as a copy of another node (clones whole tree, cannot parent)
    Node(const Node& node, Node *parent = NULL);
    
    // Construct the node from a Json blob
    Node(const Json::Value& value, Node *parent = NULL);
    
    // Standard deconstructor - deallocate all heap memory
    ~Node();
    
    // Get the name of the node
    std::string& Id();
    const std::string& Id() const;
    
    // Get a reference to the internal transformation matrix
    const glm::mat4& TransformMatrix() const;
    
    // Get a read/write reference to the transform
    Transform& LocalTransform();
    
    // Get a read only reference to the transform
    const Transform& LocalTransform() const;
    
    // Cause this node to recalculate, along with all the child nodes
    void Recalculate();
    
    // Add a child node to this node
    void AddChild(Node *child, bool recalculate = true);
    
    // Find a child node by name in this node heirarchy
    Node* FindNode(const std::string& name);
    
    // Find a child node by name in this node heirarchy (const function)
    const Node* FindNode(const std::string& name) const;
    
    // Get a reference to the children of the node
    const std::vector<Node *>& Children() const;

    // Construct a flattened map of all nodes (including this one) in the node tree
    void GetFlatNodeTree(flattreemap& table);
    void GetFlatNodeTree(const_flattreemap& table) const;
};

#endif
