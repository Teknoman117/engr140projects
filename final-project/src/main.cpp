#include <chrono>
#include <unistd.h>

#include <project/common.hpp>
#include <project/application.hpp>

void help(const char *programName)
{
    std::cout << "Usage information" << std::endl;
    std::cout << programName << " <content path>" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    // Setup clock to track shit
    std::chrono::high_resolution_clock             applicationClock;
    std::chrono::high_resolution_clock::time_point applicationStart = applicationClock.now();

    // derp
    if(argc < 2)
    {
        help(argv[0]);
    }

    // Setup parameters
    struct Application::Options options =
    {
        .contentLocation = std::string(argv[1]),
    };
    
    // Setup SDL
    SDL_Window *mainwindow;
    SDL_GLContext maincontext;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        std::cerr << "[FATAL] SDL: Unable to initialize" << std::endl;
        return 1;
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    // Setup OpenGL context settings
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Setup window manager frame buffer settings
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    
    //we use deferred rendering, no depth test in final buffer
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    
    // Setup OpenGL Accelerated Window
    mainwindow = SDL_CreateWindow("ENGR 180 Final Project - Terrain Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!mainwindow)
    {
        std::cerr << "[FATAL] SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    maincontext = SDL_GL_CreateContext(mainwindow);
    
    // Setup GLEW
    glewExperimental = GL_TRUE;
    GLenum rev = glewInit();
    if (GLEW_OK != rev || !GLEW_ARB_direct_state_access)
    {
        std::cerr << "[FATAL] " << glewGetErrorString(rev) << std::endl;
        
        SDL_GL_DeleteContext(maincontext);
        SDL_DestroyWindow(mainwindow);
        SDL_Quit();

        return 1;
    }
    
    // Enable vsync
    SDL_GL_SetSwapInterval(1);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    
    Application *app = new Application(mainwindow, maincontext, options);
    
    // Record simulation starting times, report start time
    std::chrono::high_resolution_clock::time_point simulationStart = applicationClock.now();
    std::chrono::high_resolution_clock::time_point simulationPrevious = applicationStart;
    bool                                           running = true;
    
    std::chrono::duration<float> duration = simulationStart - applicationStart;
    std::cout << "[INFO] Application up:  " << duration.count() << "s" << std::endl;
    
    // Simulatio
    while(running)
    {
        // Compute frame times
        std::chrono::high_resolution_clock::time_point simulationCurrent = applicationClock.now();
        std::chrono::duration<float>                   simulationTime    = simulationCurrent - simulationStart;
        std::chrono::duration<float>                   simulationDelta   = simulationCurrent - simulationPrevious;
        
        // Process queued events
        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                running = false;
            }
            
            else if(event.type == SDL_WINDOWEVENT)
            {
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    int w,h;
                    SDL_GetWindowSize(mainwindow,&w,&h);
                    running = app->OnResize(w, h);
                }
            }
            
            else
            {
                running = app->OnEvent(event);
            }
        }

        if(!running)
        {
            break;
        }
        
        // Render the simulation
        running = app->OnDisplay(simulationTime.count(), simulationDelta.count());
        simulationPrevious = simulationCurrent;
        SDL_GL_SwapWindow(mainwindow);
    }
    
    
    // Delete our opengl context, destroy our window, and shutdown SDL
    std::cout << "[INFO] Application terminating" << std::endl;
    
    delete app;
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
    
    return 0;
}
