#include "ow_engine.h"
#include <chrono>
#include <iostream>

bool OW_Engine::init()
{
    if (!window.create(1280, 720, "Ocean's Funky World"))
    {
        std::cout << "Failed to create window!\n";
        return false;
    }

    running = true;
    return true;
}

void OW_Engine::run()
{
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (running)
    {
        // handle windows messages
        window.pollEvents();

        // delta time
        auto currentTime = clock::now();
        float deltaTime =
            std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        processInput();
        update(deltaTime);
        render();
    }
}

void OW_Engine::shutdown()
{
    window.destroy();
    std::cout << "Engine shutdown complete.\n";
}

void OW_Engine::processInput()
{
    // input handling, key presses, etc
}

void OW_Engine::update(float deltaTime)
{
    // game logic, menu, settings, notes, etc
}

void OW_Engine::render()
{
    window.beginFrame();
    // draw stuff here
    window.endFrame();
}
