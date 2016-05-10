//
//  GLTexturedQuad.hpp
//  glutapp3d
//
//  Created by Nathaniel Lewis on 10/25/15.
//  Copyright © 2015 Nathaniel Lewis. All rights reserved.
//

#ifndef GLTexturedQuad_hpp
#define GLTexturedQuad_hpp

#include <destructo-base/stdafx.h>

#include <destructo-base/GL3DProgram.h>
#include <destructo-base/Node.h>
#include <destructo-base/GLTexture.h>
#include <destructo-base/TextureCache.h>

class GLTexturedQuad
{
    TextureCache& textureCache;
    std::string   textureName;
    
    GLuint        vertexArrayObject;
    GLuint        vertexBuffers[3];
    GLuint        textureUniform;
    
    Node         *node;
    
public:
    GLTexturedQuad(TextureCache& textureCache, std::string textureName);
    
    void Build(GL3DProgram *program, float textureTiling);
    void Draw(GL3DProgram *program);
    
    Node* GetNode();
};

#endif /* GLQuad_hpp */
