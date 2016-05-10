#include "GLCamera.h"
#include "GLCombinedMatrix.h"

GLCamera::GLCamera(GLint uniformMatrixProjection, GLint uniformMatrixView, GLint uniformMatrixCombined)
{
	this->uniformMatrixProjection = uniformMatrixProjection;
    this->uniformMatrixView = uniformMatrixView;
    this->uniformMatrixCombined = uniformMatrixCombined;
}
	

void GLCamera::CopyMatricies(mat4 * viewMatrix, mat4 * projectionMatrix) const
{
	if (viewMatrix != NULL)
		*viewMatrix = this->viewMatrix;
	if (projectionMatrix != NULL)
		*projectionMatrix = this->projectionMatrix;
}

void GLCamera::SetViewMatrix(mat4 viewMatrix) {
	this->viewMatrix = viewMatrix;	
}

void GLCamera::SetProjectionMatrix(mat4 projMatrix)
{
    this->projectionMatrix = projMatrix;
}

//Apply the camera projection/view matrix
void GLCamera::Apply()
{
    // Apply the project matrix
    if(uniformMatrixProjection >= 0)
    {
        glUniformMatrix4fv(uniformMatrixProjection, 1, GL_FALSE, &projectionMatrix[0][0]);
    }
    
    // Apply the view matrix
    if(uniformMatrixView >= 0)
    {
        glUniformMatrix4fv(uniformMatrixView, 1, GL_FALSE, &viewMatrix[0][0]);
    }
    
    // Apply the precomputed matrix if required
    if(uniformMatrixCombined >= 0)
    {
        mat4 combined = projectionMatrix * viewMatrix;
        glUniformMatrix4fv(uniformMatrixCombined, 1, GL_FALSE, &combined[0][0]);
    }
}

//Reset the view and projection matrix to the identity matrix
void GLCamera::Reset() {
	viewMatrix = mat4();
	projectionMatrix = mat4();
}

//Set the camera position in the world. Up vector defaults to 0,0,1
void GLCamera::SetCameraPosition(vec3 cameraPos, vec3 target, vec3 upVector)
{
    viewMatrix = glm::lookAt(cameraPos, target, upVector);
}

vec3 GLCamera::GetZAxis() {
	return cameraZAxis;
}


//Change the frustrum matrix to the given values
void GLCamera::SetFrustrum(float verticalFOV, float aspectRatio, float nearDistance, float farDistance)
{
	projectionMatrix = glm::perspective(verticalFOV, aspectRatio, nearDistance, farDistance);
}

// Get the view matrix
const mat4& GLCamera::ViewMatrix() const
{
    return viewMatrix;
}

// Get the projection matrix
const mat4& GLCamera::ProjectionMatrix() const
{
    return projectionMatrix;
}