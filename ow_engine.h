#pragma once
#include "ow_window.h"

class OW_Engine
{
public:
    bool init();
    void run();
    void shutdown();

private:
    void processInput();
    void update(float deltaTime);
    void render();

    bool running = false;
    OW_Window window; // our window layer
};
