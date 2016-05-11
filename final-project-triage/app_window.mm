# include <destructo-base/OS.h>
# include "app_window.h"

# if defined __APPLE__
# import <Foundation/Foundation.h>
# endif

namespace
{
    // Compute the shadow matrix (adapted from ftp://ftp.sgi.com/opengl/contrib/blythe/advanced99/notes/node192.html)
    void ComputeShadowMatrixPointLight(mat4& shadowMat, vec4 light, vec4 ground)
    {
        mat4 dotmat = mat4(1.0) * glm::dot(light, ground);
        
        shadowMat[0] = light * ground[0];
        shadowMat[1] = light * ground[1];
        shadowMat[2] = light * ground[2];
        shadowMat[3] = light * ground[3];
        
        shadowMat = dotmat - shadowMat;
    }
}

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
    : GlutWindow ( label, x, y, w, h ), 
      sceneRoot(new Node()), 
      modelGroup("meshes/manifest.json", textureCache)
{
    initPrograms ();
    
    sceneRoot->Id() = "Scene Root";
    
    // Create an instance of the mechwarrior model
    mechwarriorInstance = modelGroup.NewInstance("robot02");
    mechwarriorInstance->GetNode()->Id() = "Mechwarrior";
    sceneRoot->AddChild(mechwarriorInstance->GetNode());
    
    // Create a textured quad for the ground (tile the texture 50 times across its surface
    ground = new GLTexturedQuad(textureCache, "textures/terrain.png");
    ground->Build(groundProgram, 50.0);
    ground->GetNode()->LocalTransform().Scale() = vec3(100,1,100);
    ground->GetNode()->LocalTransform().Translation() = vec3(0,-0.001,0);
    ground->GetNode()->Id() = "Ground";
    sceneRoot->AddChild(ground->GetNode());
    
    // Setup the cameras
    mechCamera = new Node();
    mechCamera->LocalTransform().Rotation() = glm::angleAxis(-glm::pi<float>(), vec3(0,1,0));
    mechCamera->LocalTransform().Translation() = glm::vec3(0,0.35,-0.95);
    mechCameraRotator = new Node();
    mechCameraRotator->LocalTransform().Translation() = glm::vec3(0,0.5,0);
    mechCameraRotator->AddChild(mechCamera);
    mechwarriorInstance->GetNode()->AddChild(mechCameraRotator);

    enemies.push_back(new Enemy(modelGroup, sceneRoot, glm::vec3(5, 0, 5), glm::vec3(5, 0, -5)));
    enemies.push_back(new Enemy(modelGroup, sceneRoot, glm::vec3(-5, 0, -5), glm::vec3(-5, 0, 5)));
    enemies.push_back(new Enemy(modelGroup, sceneRoot, glm::vec3(-2, 0, -2), glm::vec3(-3.1, 0, 7.1)));
    enemies.push_back(new Enemy(modelGroup, sceneRoot, glm::vec3(2, 0, -2), glm::vec3(3.1, 0, 7.1)));
    enemies.push_back(new Enemy(modelGroup, sceneRoot, glm::vec3(-2, 0, 2), glm::vec3(-3.1, 0, -7.1)));
    
    // Initial state for some tracking
    keys['w'] = false;
    keys['a'] = false;
    keys['s'] = false;
    keys['d'] = false;
    
    rotation = 0.0f;
    show = 0;
}

void AppWindow::initPrograms ()
{
    // Load the program for the model shader
    modelProgram = new MaterialProgram("shaders/vsh_model.glsl", "shaders/fsh_model.glsl");
    groundProgram = new GL3DProgram("shaders/vsh_quad.glsl", "shaders/fsh_quad.glsl");
    lineProgram = new GL3DProgram("shaders/vsh_flat.glsl", "shaders/fsh_flat.glsl");
    
    // Setup the shadow matrix for the light
    mat4 shadowMatrix;
    ComputeShadowMatrixPointLight(shadowMatrix, vec4(0,3,0,1), glm::vec4(0,1,0,0));
    modelProgram->SetShadowMatrix(shadowMatrix);
    groundProgram->SetShadowMatrix(shadowMatrix);
    
    // Setup the line rendering
    float line[12] = {0, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, -500};
    
    glGenVertexArrays(1, &linevao);
    glGenBuffers(1, &linebuf);
    
    glBindBuffer(GL_ARRAY_BUFFER, linebuf);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), &line[0], GL_STATIC_DRAW );
}

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
vec2 AppWindow::windowToScene ( const vec2& v )
{
    // GsVec2 is a lighteweight class suitable to return by value:
    return vec2 ( (2.0f*(v.x/(viewport.x))) - 1.0f,
                   1.0f - (2.0f*(v.y/(viewport.y))) );
}

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
{
    // Check if we should trigger oneshot
    auto tracker = keys.find(key);
    if(tracker == keys.end() || !tracker->second)
    {
        keys[key] = true;
        OnKeyDown(key);
    }
}

void AppWindow::glutKeyboardUp ( unsigned char key, int x, int y )
{
    keys[key] = false;
    mechwarriorAnimationControl();
}

void AppWindow::OnKeyDown(unsigned char key)
{
    switch ( key )
    {
        case 27 : exit(1); // Esc was pressed
        case 'w':
        case 'a':
        case 's':
        case 'd':
            mechwarriorAnimationControl();
    }
}

void AppWindow::glutSpecial ( int key, int x, int y )
{
    auto tracker = specials.find(key);
    if(tracker == specials.end() || !tracker->second)
    {
        specials[key] = true;
        OnSpecialDown(key);
    }
}

void AppWindow::glutSpecialUp ( int key, int x, int y )
{
}

void AppWindow::OnSpecialDown(int special)
{
}

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.

 }

void AppWindow::glutMotion ( int x, int y )
{
}

void AppWindow::glutPassiveMotion ( int x, int y )
{
    static int lastX = viewport.x/2;
    static int lastY = viewport.y/2;
    
    int deltaX = x - lastX;
    int deltaY = y - lastY;
    
    lastX = x;
    lastY = y;
    
    if( deltaX == 0 && deltaY == 0 ) return;
    
    int windowX		= glutGet( GLUT_WINDOW_X );
    int windowY		= glutGet( GLUT_WINDOW_Y );
    int screenWidth		= glutGet( GLUT_SCREEN_WIDTH );
    int screenHeight	= glutGet( GLUT_SCREEN_HEIGHT );
    
    int screenLeft = -windowX;
    int screenTop = -windowY;
    int screenRight = screenWidth - windowX;
    int screenBottom = screenHeight - windowY;
    
    if( x <= screenLeft+10 || (y) <= screenTop+10 || x >= screenRight-10 || y >= screenBottom - 10)
    {
        //std::cout << x << " " << y << std::endl;
        lastX = viewport.x/2;
        lastY = viewport.y/2;
        
#if defined __APPLE__
        //	If on Mac OS X, the following will also work (and CGwarpMouseCursorPosition seems faster than glutWarpPointer).
        	CGPoint centerPos = CGPointMake( windowX + lastX, windowY + lastY );
        	CGWarpMouseCursorPosition( centerPos );
        // Have to re-hide if the user touched any UI element with the invisible pointer, like the Dock.
        	CGDisplayHideCursor(kCGDirectMainDisplay);
#else
        glutWarpPointer( lastX, lastY );
        glutSetCursor(GLUT_CURSOR_NONE);
#endif
    }
    
    mouseCounter += vec2(-deltaX, deltaY) / 3.f;
}

void AppWindow::glutMenu ( int m )
{
}

void AppWindow::glutReshape ( int w, int h )
{
    // Update the projection matrix with the new viewport information
    viewport = vec2(w,h);
    modelProgram->Camera.SetFrustrum(glm::pi<float>() / 3.0f, viewport.x / viewport.y, 0.1f, 50.0f);
    groundProgram->Camera.SetFrustrum(glm::pi<float>() / 3.0f, viewport.x / viewport.y, 0.1f, 50.0f);
    lineProgram->Camera.SetFrustrum(glm::pi<float>() / 3.0f, viewport.x / viewport.y, 0.1f, 50.0f);
    
    // Define that OpenGL should use the whole window for rendering
    glViewport( 0, 0, w, h );
}

void AppWindow::mechwarriorAnimationControl()
{
    static int currentState = -1;

    if(keys['w'])
    {
        mechwarriorRate.x = 0.65;
        if(currentState != GLUT_KEY_UP)
            mechwarriorInstance->PlayAnimation("walking");
        currentState = GLUT_KEY_UP;
    } else if(keys['s'])
    {
        mechwarriorRate.x = -0.40;
        if(currentState != GLUT_KEY_DOWN)
            mechwarriorInstance->PlayAnimation("backpedal");
        currentState = GLUT_KEY_DOWN;
    } else mechwarriorRate.x = 0.0f;

    if(keys['a'])
    {
        mechwarriorRate.y = glm::pi<float>() / 6.0f;

        if(!keys['w'] && !keys['s'])
        {
            if(currentState != GLUT_KEY_LEFT)
                mechwarriorInstance->PlayAnimation("strafe_left");
            currentState = GLUT_KEY_LEFT;
        }
    } else if(keys['d'])
    {
        mechwarriorRate.y = -glm::pi<float>() / 6.0f;

        if(!keys['w'] && !keys['s'])
        {
            if(currentState != GLUT_KEY_RIGHT)
                mechwarriorInstance->PlayAnimation("strafe_right");
            currentState = GLUT_KEY_RIGHT;
        }
    } else mechwarriorRate.y = 0.0f;

    // null animation
    if(fabs(mechwarriorRate.x) < 0.01f && fabs(mechwarriorRate.y) < 0.01f)
    {
        mechwarriorRate = vec2(0,0);
        if(currentState != -1)
            mechwarriorInstance->PlayNullAnimation();
        currentState = -1;
    }
}

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
{
    // Refesh the texture cache
    textureCache.Refresh();
    
    // Compute the frame delta
    static double previousTime = OS::Now();
    double        currentTime  = OS::Now();
    double        delta        = currentTime - previousTime;
    
    // Update mechwarrior pose
    mechwarriorCamera += mouseCounter * (float) delta;
    mechwarriorCamera.y = clamp<float>(mechwarriorCamera.y, -glm::pi<float>()/4.0f, glm::pi<float>()/4.0f);
    mouseCounter = vec2(0,0);

    vec2 maximum = vec2(glm::pi<float>()/2.f, glm::pi<float>()/2.f) * (float) delta;
    vec2 minimum = vec2(-glm::pi<float>()/2.f, -glm::pi<float>()/2.f) * (float) delta;
    vec2 response = clamp(mechwarriorCamera - mechwarriorPose, minimum, maximum);
    mechwarriorPose += response;

    // Update mechwarrior global motion
    mechwarriorInstance->Update(delta, currentTime);
    std::for_each(enemies.begin(), enemies.end(), [&] (Enemy *e) { e->Update(delta, currentTime); });
    if(fabs(mechwarriorRate.x) > 0.0f || fabs(mechwarriorRate.y) > 0.0f)
    {
        mat4 currentUpdate = glm::rotate(mechwarriorRate.y * (float) delta, vec3(0.0f,1.0f,0.0f)) 
                           * glm::translate(vec3(mechwarriorRate.y, 0, mechwarriorRate.x) * (float) delta);

        mat4 currentTransform = mechwarriorInstance->GetNode()->LocalTransform().TransformMatrix();
        mechwarriorInstance->GetNode()->LocalTransform().SetTransformMatrix(currentTransform * currentUpdate);
    }
    
    // Update manual animation goals for all animations
    mechwarriorInstance->Animation()->Skeleton()->FindNode("body")->LocalTransform().Rotation() *= glm::angleAxis(mechwarriorPose.x, vec3(0,1,0));
    mechwarriorInstance->Animation()->Skeleton()->FindNode("lf_elbow")->LocalTransform().Rotation() *= glm::angleAxis(mechwarriorPose.y, vec3(1,0,0));
    mechwarriorInstance->Animation()->Skeleton()->FindNode("rt_elbow")->LocalTransform().Rotation() *= glm::angleAxis(mechwarriorPose.y, vec3(1,0,0));
    mechwarriorInstance->Animation()->Skeleton()->FindNode("lf_wrist")->LocalTransform().Rotation() *= glm::angleAxis(rotation, vec3(0,0,1));
    mechwarriorInstance->Animation()->Skeleton()->FindNode("rt_wrist")->LocalTransform().Rotation() *= glm::angleAxis(rotation, vec3(0,0,1));
    mechwarriorInstance->Animation()->Skeleton()->Recalculate();

    // Update player camera
    mechCameraRotator->LocalTransform().Rotation() = glm::angleAxis(mechwarriorCamera.x, vec3(0,1,0)) * glm::angleAxis(mechwarriorCamera.y, vec3(1,0,0));
    
    // Update scene graph
    sceneRoot->Recalculate();

    // Check if we should kill any of them
    if(keys[' '])
    {
        rotation += 4*glm::pi<float>() * delta;
        std::for_each(enemies.begin(), enemies.end(), [&] (Enemy *e) 
        {
            if(!e->alive)
                return;

            // compute the direction to it
            vec3 dir = normalize(e->model->GetNode()->LocalTransform().Translation() - mechwarriorInstance->GetNode()->LocalTransform().Translation());
            // compute viewing vector

            Transform t;
            t.SetTransformMatrix(mechCameraRotator->TransformMatrix());
            vec3 me = t.Rotation() * vec3(-1,0,0);
            me = normalize(cross(vec3(0,1,0), me));
            float d = dot(me, dir);

            //std::cout << me.x << " " << me.y << " " << me.z << " " << d << std::endl;
            if(d > 0.999f)
            {
                e->health = e->health - delta;
                std::cout << "health = " << e->health << std::endl;
                if(e->health < 0)
                    e->Die();
            }
        });
    }
    
    // Compute the camera matrix
    mat4 viewMatrix = inverse(mechCamera->TransformMatrix());

    groundProgram->Camera.SetViewMatrix(viewMatrix);
    modelProgram->Camera.SetViewMatrix(viewMatrix);
    lineProgram->Camera.SetViewMatrix(viewMatrix);
    
    // Perform all renderering
    glClearColor(0.1,0.1,0.1,1.0);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Render the ground
    groundProgram->UseProgram();
    ground->Draw(groundProgram);
    
    if(keys[' '])
    {
        // Render the firing
        lineProgram->UseProgram();
        
        if(!(++show % 6))
        {
            glBindVertexArray(linevao);
            glBindBuffer ( GL_ARRAY_BUFFER, linebuf ); // positions
            glEnableVertexAttribArray ( 0 );
            glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
        
            mat4 rt = mechwarriorInstance->GetNode()->TransformMatrix() * mechwarriorInstance->Animation()->Skeleton()->FindNode("rt_wrist")->TransformMatrix();
            lineProgram->Model.Reset();
            lineProgram->Model.SetMatrix(rt);
            lineProgram->Apply();
            
            glDrawArrays(GL_LINES, 0, 2);
            
            mat4 lf = mechwarriorInstance->GetNode()->TransformMatrix() * mechwarriorInstance->Animation()->Skeleton()->FindNode("lf_wrist")->TransformMatrix();
            lineProgram->Model.Reset();
            lineProgram->Model.SetMatrix(lf);
            lineProgram->Apply();
            
            glDrawArrays(GL_LINES, 2, 2);
        }
    }
    
    // Render the mechwarrior
    modelProgram->UseProgram();
    modelProgram->SetShadowMode(false);
    mechwarriorInstance->Draw(modelProgram);
    modelProgram->SetShadowMode(true);
    mechwarriorInstance->Draw(modelProgram);

    std::for_each(enemies.begin(), enemies.end(), [&] (Enemy *e)
    {
        e->Draw(modelProgram);
    });
    
    previousTime = currentTime;
    glutSwapBuffers();
}

void AppWindow::glutIdle()
{
    redraw();
}

