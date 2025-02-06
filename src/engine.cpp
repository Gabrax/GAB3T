#include "engine.h"
#include "Core/Backend/window.h"
#include "Core/Game/GameLogic.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Core/Backend/Bloom.h"

void Engine::Run()
{
    Window::Init((int)(800),(int)(600));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Game game;
    FrameBuffer fb;

    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::BeginFrame();

          fb.Bind(); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          game.Update();
          fb.RenderBloomTexture(0.005f);
          fb.UnBind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          fb.Render();

        Window::EndFrame();
    }

#ifdef DEBUG
    game.printTuple();
#endif
}
