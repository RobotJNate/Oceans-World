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
    OW_Window window; // window layer

    // rectangle test
    float rectX = 100.0f;
    float rectY = 100.0f;
    float rectWidth = 50.0f;
    float rectHeight = 50.0f;
};
