#include "GL3DProgram.h"

GL3DProgram::GL3DProgram(string vertexShaderPath, string fragmentShaderPath) :
	GLProgram(vertexShaderPath,fragmentShaderPath),
	Lights(programId),
	Model(glGetUniformLocation(programId,"M")),
	Camera(glGetUniformLocation(programId,"P"),glGetUniformLocation(programId,"V"),glGetUniformLocation(programId,"VP"))
{

	attributeIndexTexture = glGetAttribLocation(programId,"vTex");
	attributeIndexPosition = glGetAttribLocation(programId,"vPos");
	attributeIndexVertex = glGetAttribLocation(programId,"vVert");
	attributeIndexNormal = glGetAttribLocation(programId,"vNorm");
    uniformMaterialReflectivity = glGetUniformLocation(programId, "material_reflectivity");
    uniformMatrixModelView = glGetUniformLocation(programId, "MV");
    uniformMatrixModelViewProjection = glGetUniformLocation(programId, "MVP");
    uniformModeShadow = glGetUniformLocation(programId, "shadowMode");
    
    shadowMode = false;
    shadowMatrix = mat4(1.0f);
};

void GL3DProgram::UseProgram(void) {
	GLProgram::UseProgram();
}

const GLint GL3DProgram::AttributePosition() {
	return attributeIndexPosition;
}
const GLint GL3DProgram::AttributeTexture() {
	return attributeIndexTexture;
}
const GLint GL3DProgram::AttributeVertex() {
	return attributeIndexVertex;
}
const GLint GL3DProgram::AttributeNormal() {
	return attributeIndexNormal;
}
const GLint GL3DProgram::UniformMaterialReflectivity()
{
    return uniformMaterialReflectivity;
}

void GL3DProgram::Apply()
{
    // Work in progress matrix
    mat4 storage = Model.GetMatrix();
    
    // If we have a model view matrix
    if(uniformMatrixModelView >= 0)
    {
        storage = Camera.ViewMatrix() * (shadowMode ? shadowMatrix * storage : storage);
        glUniformMatrix4fv(uniformMatrixModelView, 1, GL_FALSE, &storage[0][0]);
    }
    
    // If we have a model view projection matrix
    if(uniformMatrixModelViewProjection >= 0)
    {
        // If the matrix was already premultiplied
        if(uniformMatrixModelView >= 0)
        {
            storage = Camera.ProjectionMatrix() * storage;
        } else
        {
            // Do the full chain
            storage = Camera.ProjectionMatrix() * Camera.ViewMatrix() * (shadowMode ? shadowMatrix * storage : storage);
        }
        
        // Set the mvp matrix
        glUniformMatrix4fv(uniformMatrixModelViewProjection, 1, GL_FALSE, &storage[0][0]);
    }
    
    glUniform1i(uniformModeShadow, shadowMode ? 1 : 0);
    
    // Apply other stuff
    Model.Apply();
    Camera.Apply();
    //Lights.Apply();
}

void GL3DProgram::SetShadowMatrix(mat4 shadowMatrix)
{
    this->shadowMatrix = shadowMatrix;
}

void GL3DProgram::SetShadowMode(bool shadowMode)
{
    this->shadowMode = shadowMode;
}
