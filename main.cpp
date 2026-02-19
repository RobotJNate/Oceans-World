#include "ow_engine.h"

int main()
{
    OW_Engine engine;

    if (!engine.init())
        return -1;

    engine.run();
    engine.shutdown();

    return 0;
}
