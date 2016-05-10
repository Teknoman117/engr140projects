#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <project/resources.hpp>
#include <project/renderpipeline.hpp>
#include <project/cameraparameters.hpp>
#include <project/modelinstance.hpp>

#include <project/objects/glfullscreenquad.hpp>
#include <project/objects/glsphere.hpp>
#include <project/objects/glcube.hpp>

#include <vector>

class Application : public Resources
{
public:
    struct Options
    {
        std::string contentLocation;
    };

private: 
    SDL_Window               *window;
    SDL_GLContext&            context;
    SDL_Joystick             *joystick;

    CameraParameters          camera;
    float                     timeOfDay;

    RenderPipeline            renderer;
    GLFullscreenQuad          directionalLight;

    std::unique_ptr<GLSphere>      sphere;
    std::unique_ptr<GLCube>        skybox;
    std::unique_ptr<ModelInstance> player;
    
public:
    Application(SDL_Window *window, SDL_GLContext& context, const Application::Options& options);
    ~Application();
    
    bool OnDisplay(float frameTime, float frameDelta);
    bool OnResize(GLint width, GLint height);
    bool OnEvent(SDL_Event event);
};

#endif
