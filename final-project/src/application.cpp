//
//  Application.cpp
//  Application
//
//  Created by Nathaniel Lewis on 11/21/14.
//  Copyright (c) 2014 HoodooNet. All rights reserved.
//

#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>

#include <project/application.hpp>

Application::Application(SDL_Window *_window, SDL_GLContext& _context, const Application::Options& options)
    : Resources(options.contentLocation, "manifest.json"), window(_window), context(_context), renderer(*this)
{
    SDL_GetWindowSize(window, &camera.width, &camera.height);
    
    if (SDL_NumJoysticks() > 0)
    {
        joystick = SDL_JoystickOpen(0);
        SDL_JoystickEventState(SDL_ENABLE);
    }

    // Initialize view position
    camera.position = glm::vec3(0,0,-10);
    camera.rotation = glm::vec2(0,0);

    timeOfDay = 2;

    // allocate pipeline buffers
    OnResize(camera.width, camera.height);
}

Application::~Application()
{
}

bool Application::OnDisplay(float frameTime, float frameDelta)
{
    // Update window title with fps
    std::ostringstream windowTitle;
    windowTitle << std::setprecision(2) << "ENGR 140 Final Project - FPS = " << (1.f / frameDelta);
    SDL_SetWindowTitle(window, windowTitle.str().c_str());

    // Compute base movement vector
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    /*glm::vec3 moveDirection;

    if(state[SDL_SCANCODE_W])
        moveDirection.z = -1.f;
    else if(state[SDL_SCANCODE_S])
        moveDirection.z = 1.f;

    if(state[SDL_SCANCODE_A])
        moveDirection.x = -1.0f;
    else if(state[SDL_SCANCODE_D])
        moveDirection.x = 1.f;

    if(state[SDL_SCANCODE_R])
        moveDirection.y = 1.0f;
    else if(state[SDL_SCANCODE_F])
        moveDirection.y = -1.f;*/

    if(state[SDL_SCANCODE_PAGEUP] || (joystick && ::SDL_JoystickGetButton(joystick, 5)))
        timeOfDay += 1.0 * frameDelta;
    else if(state[SDL_SCANCODE_PAGEDOWN] || (joystick && ::SDL_JoystickGetButton(joystick, 4)))
        timeOfDay -= 1.0 * frameDelta;

    /*if(state[SDL_SCANCODE_HOME] || (joystick && ::SDL_JoystickGetButton(joystick, 7)))
        animationSpeed += 50.0f * frameDelta;
    else if(state[SDL_SCANCODE_END] || (joystick && ::SDL_JoystickGetButton(joystick, 6)))
        animationSpeed = glm::max<float>(0.0f, animationSpeed - 50.f * frameDelta);

    // Apply controls from a joystick
    if (joystick != NULL)
    {
        const float threshold = 0.25f;
        const float joyStickNoramlizationFactor = 32768.0f;
        const float cameraSpeed = -138.75f;

        // SDL returns a short between its min and max vals.
        float movementX = SDL_JoystickGetAxis(joystick, 0) / joyStickNoramlizationFactor;
        float movementZ = SDL_JoystickGetAxis(joystick, 1) / joyStickNoramlizationFactor;

        // Override with control inputs if above a certain threshold
        if (movementX < -threshold || movementX > threshold)
        {
            moveDirection.x = movementX;
        }
        if (movementZ < -threshold || movementZ > threshold)
        {
            moveDirection.z = movementZ;
        }

        float cameraMovementX = SDL_JoystickGetAxis(joystick, 2) / joyStickNoramlizationFactor;
        float cameraMovementZ = SDL_JoystickGetAxis(joystick, 5) / joyStickNoramlizationFactor;

        if (cameraMovementX < -threshold || cameraMovementX > threshold)
        {
            camera.rotation.y += cameraMovementX * (cameraSpeed * frameDelta) * glm::pi<float>() / 180.f;
        }
        if (cameraMovementZ < -threshold || cameraMovementZ > threshold)
        {
            camera.rotation.x += cameraMovementZ * (cameraSpeed * frameDelta) * glm::pi<float>() / 180.f;
        }

        camera.rotation.x = glm::clamp(camera.rotation.x, -85.0f * glm::pi<float>() / 180.f, 85.0f * glm::pi<float>() / 180.f);
    }

    // spline following camera
    if(following)
    {
        animationTime += frameDelta * (animationSpeed / distance(cameraPath[currentPosition], cameraPath[currentPosition+1]));

        // Handle next frame / end logic
        if(animationTime > 1.0)
        {
            int overage = floor(animationTime);
            currentPosition += overage;
            animationTime -= overage;

            if(currentPosition >= cameraPath.size()-1)
            {
                following = false;
                animationTime = 0.f;
                currentPosition = 0;
            }
        }
        
        // If we are still following (didn't bail)
        if(following)
        {
            camera.position = mix(cameraPath[currentPosition], cameraPath[currentPosition+1], animationTime);
        }
    }

    // jogging or sprinting
    else if(!flying)
    {
        moveDirection *= (state[SDL_SCANCODE_LSHIFT] || (joystick && ::SDL_JoystickGetButton(joystick, 10)) ? 6.7056f : 2.68224f);

        // Update position in the current view direction
        camera.position += (glm::angleAxis(camera.rotation.y, glm::vec3(0.f,1.f,0.f)) * moveDirection) * frameDelta;
        camera.position.y = chunkedTerrain->GetElevationAt(glm::vec3(camera.position.x, 0, camera.position.z)) + 1.8f;
    }

    // flying camera
    else
    {
        moveDirection *= (state[SDL_SCANCODE_LSHIFT] || (joystick && ::SDL_JoystickGetButton(joystick, 10)) ? 240.0f : 30.f);

        glm::vec3 forward = camera.RotationAsQuaternion()*glm::vec3(0,0,1);
        glm::vec3 right   = cross(forward, camera.up);    
        glm::vec3 lup     = cross(right, forward);

        camera.position += -moveDirection.x*right*frameDelta + 
                            moveDirection.y*lup*frameDelta +
                            moveDirection.z*forward*frameDelta;

        float h = chunkedTerrain->GetElevationAt(camera.position);
        if(camera.position.y < (h + 1.8f) && h != std::numeric_limits<float>::infinity()) 
            camera.position.y = h + 1.8f;
    }*/

    // Update camera uniform buffer
    camera.Update();

    // Start rendering
    renderer.BeginRendering();
    {
        // --------------- OBJECT DRAWING PHASE ----------------------------
        renderer.BeginGBufferPass();
        {
            // Skybox
            GetTexture("skybox").Bind(GL_TEXTURE0);
            GetProgram("skybox").Bind();
            glUniform1i(GetProgram("skybox").GetUniform("uTexture"), 0);

            glDepthRangef(0.9999999f,1);
            directionalLight.Draw();
            glDepthRangef(0,1);

            // Terrain
            /*textures["grass_diffuse"]->Bind(GL_TEXTURE1);
            textures["cliff_diffuse"]->Bind(GL_TEXTURE2);
            textures["grass_normals"]->Bind(GL_TEXTURE3);
            textures["cliff_normals"]->Bind(GL_TEXTURE4);

            programs["terrain"]->Bind();

            glUniform1i(programs["terrain"]->GetUniform("horizontalTexture"),        1);
            glUniform1i(programs["terrain"]->GetUniform("verticalTexture"),          2);
            glUniform1i(programs["terrain"]->GetUniform("horizontalTextureNormals"), 3);
            glUniform1i(programs["terrain"]->GetUniform("verticalTextureNormals"),   4);
            glUniform1i(programs["terrain"]->GetUniform("shadingEnabled"),           shadingEnabled);
            glUniform1i(programs["terrain"]->GetUniform("tessellationEnabled"),      tessellationEnabled);
            glUniform1i(programs["terrain"]->GetUniform("fractalsEnabled"),          fractalsEnabled);

            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            chunkedTerrain->Draw(programs["terrain"]);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Path line
            if(pathLine && pathLineEnabled)
            {
                programs["diffuse"]->Bind();
                glUniform3f(programs["diffuse"]->GetUniform("DiffuseColor"), 10.f, 10.f, 10.f);
                glUniform1f(programs["diffuse"]->GetUniform("SpecularExponent"), 32.f);
                glm::mat4 nulltransform;
                glUniformMatrix4fv(programs["diffuse"]->GetUniform("M"), 1, GL_FALSE, glm::value_ptr(nulltransform));
                pathLine->Draw();
            }*/
        }
        renderer.EndGBufferPass();


        // --------------- LIGHT DRAWING PHASE -----------------------------
        renderer.BeginLightPass();
        {
            // Light direction in view space
            // Compute the position of a light
            glm::vec3 ld = glm::angleAxis(-glm::pi<float>()*35.0f / 180.0f, glm::vec3(1,0,0)) * 
                           glm::vec3(sin(timeOfDay),
                                     cos(timeOfDay),
                                     0);

            // Directional Lights
            GetProgram("directionalLight").Bind();

            glUniform3f (GetProgram("directionalLight").GetUniform("LightColor"),     1.0f, 1.0f, 1.0f);
            glUniform3f (GetProgram("directionalLight").GetUniform("LightIntensity"), 0.2f, 6.0f, 0.0f);
            glUniform3fv(GetProgram("directionalLight").GetUniform("LightDirection"), 1, glm::value_ptr(ld));

            glUniform1i(GetProgram("directionalLight").GetUniform("gBufferDiffuseSpecular"), 0);
            glUniform1i(GetProgram("directionalLight").GetUniform("gBufferNormals"), 1);
            glUniform1i(GetProgram("directionalLight").GetUniform("gBufferPosition"), 2);

            renderer.BeginGlobalLightRender();
            directionalLight.Draw();
            renderer.EndGlobalLightRender();
        }
        renderer.EndLightPass();
    }
    renderer.EndRendering();

    // Continue application
    return true;
}

bool Application::OnResize(GLint _width, GLint _height)
{
    std::cout << "[INFO] window resized " << _width << ", " << _height << std::endl;

    camera.width    = _width;
    camera.height   = _height;
    camera.aspect   = float(_width) / float(_height);
    camera.viewport = glm::vec4(0, 0, _width, _height);

    // Reallocate framebuffer storage
    renderer.ResizeRenderPipeline(camera.width, camera.height);

    return true;
}

bool Application::OnEvent(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        // ------------------ KEYBOARD CONTROLS --------------------------
        if(event.key.keysym.sym == SDLK_ESCAPE)
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        /*else if(event.key.keysym.sym == SDLK_SPACE)
        {
            wireframe = !wireframe;
        }
        else if(event.key.keysym.sym == SDLK_i)
        {
            following = false;
            flying =  false;
        }
        else if(event.key.keysym.sym == SDLK_o)
        {
            following = false;
            flying = true;
        }
        else if((event.key.keysym.sym == SDLK_p) && cameraPath.size())
        {
            following = true;
            currentPosition = 0;
            animationTime = 0.f;
        }
        else if(event.key.keysym.sym == SDLK_v)
        {
            pathLineEnabled = !pathLineEnabled;
        }
        else if(event.key.keysym.sym == SDLK_b)
        {
            fractalsEnabled = !fractalsEnabled;
        }
        else if(event.key.keysym.sym == SDLK_n)
        {
            tessellationEnabled = !tessellationEnabled;
        }
        else if(event.key.keysym.sym == SDLK_m)
        {
            shadingEnabled = !shadingEnabled;
        }*/
    }

    /*else if(event.type == SDL_JOYBUTTONDOWN)
    {
        // ------------ JOYSTICK BUTTONS --------------
        if(event.jbutton.button == 12)
        {
            wireframe = !wireframe;
        }
        else if(event.jbutton.button == 0)
        {
            following = false;
            flying =  false;
        }
        else if(event.jbutton.button == 1)
        {
            following = false;
            flying = true;
        }
        else if((event.jbutton.button == 2) && cameraPath.size())
        {
            following = true;
            currentPosition = 0;
            animationTime = 0.f;
        }
    }

    else if(event.type == SDL_JOYHATMOTION)
    {
        // ------------- JOYSTICK AXES -------------------
        if(event.jhat.value == 8)
        {
            fractalsEnabled = !fractalsEnabled;
        }
        else if(event.jhat.value == 4)
        {
            tessellationEnabled = !tessellationEnabled;
        }
        else if(event.jhat.value == 2)
        {
            shadingEnabled = !shadingEnabled;
        }
        else if(event.jhat.value == 1)
        {
            pathLineEnabled = !pathLineEnabled;
        }
    }*/

    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if(SDL_GetRelativeMouseMode() == SDL_FALSE)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        else
        {
            // Do something cool otherwise
        }
    }

    // If the mouse is captured and it moves, move the camera
    else if(event.type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode() == SDL_TRUE)
    {
        camera.rotation.y += -0.025f * float(event.motion.xrel) * glm::pi<float>() / 180.f;
        camera.rotation.x += -0.025f * float(event.motion.yrel) * glm::pi<float>() / 180.f;

        camera.rotation.x = glm::clamp(camera.rotation.x, -85.0f * glm::pi<float>() / 180.f, 85.0f * glm::pi<float>() / 180.f);
    }

    return true;
}
