#pragma once
#include "ow_window.h"

class OW_Engine
{
public:
    bool init();
    void run();
    void shutdown();

    void drawRect(float x, float y, float width, float height, int r, int g, int b);
private:
    void processInput();
    void update(float deltaTime);
    void render();

    bool running = false;
    OW_Window window;

    // rectangle test
    float rectX = 100.0f;
    float rectY = 100.0f;
    float rectWidth = 50.0f;
    float rectHeight = 50.0f;
};

