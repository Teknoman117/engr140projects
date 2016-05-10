//
//  GLTexturedQuad.cpp
//  glutapp3d
//
//  Created by Nathaniel Lewis on 10/25/15.
//  Copyright © 2015 Nathaniel Lewis. All rights reserved.
//

#include "GLTexturedQuad.hpp"

GLTexturedQuad::GLTexturedQuad(TextureCache& textureCache, std::string textureName)
    : textureCache(textureCache), textureName(textureName), node(new Node())
{
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(3, &vertexBuffers[0]);
}

void GLTexturedQuad::Build(GL3DProgram *program, float textureTiling)
{
    static vec3 positions[] =
    {
        // first triangle
        vec3(-0.5,0,-0.5),
        vec3(-0.5, 0, 0.5),
        vec3(0.5, 0, -0.5),
        
        // second triangle
        vec3(0.5, 0, 0.5),
        vec3(0.5, 0, -0.5),
        vec3(-0.5, 0, 0.5),
    };
    
    static vec3 normals[] =
    {
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
    };
    
    static vec2 texcoords[] =
    {
        vec2(0,0),
        vec2(0,textureTiling),
        vec2(textureTiling,0),
        
        vec2(textureTiling, textureTiling),
        vec2(textureTiling, 0),
        vec2(0, textureTiling),
    };
    
    glBindVertexArray(vertexArrayObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(program->AttributeVertex());
    glVertexAttribPointer(program->AttributeVertex(), 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(program->AttributeNormal());
    glVertexAttribPointer(program->AttributeNormal(), 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), &texcoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(program->AttributeTexture());
    glVertexAttribPointer(program->AttributeTexture(), 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    
    textureUniform = glGetUniformLocation(program->GetId(), "diffuse");
}

void GLTexturedQuad::Draw(GL3DProgram *program)
{
    // Setup the transformation matrices
    program->Model.PushMatrix();
    program->Model.SetMatrix(node->TransformMatrix());
    program->Apply();
    program->Model.PopMatrix();
    
    // Get the texture to use for the terrain
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureUniform, 0);
    textureCache.GetTexture<GLTexture>(textureName)->Bind();
    
    // Draw teh terrain
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Node* GLTexturedQuad::GetNode()
{
    return node;
}
