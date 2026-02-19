bool OW_Engine::init()
{
    if (!window.create(1280, 720, "Ocean's Funky World"))
    {
        return false;
    }

    // MAIN MENU
    mainMenuItems = {
        {"Story Mode", true, true}, // * placeholder
        {"Freeplay", false, true},  // locked
        {"Settings", true, false},
        {"Exit", true, false}
    };

    // SETTINGS MENU
    settingsMenuItems = {
        {"Volume: 3", true, false}, 
        {"Downscroll: OFF", true, true}, 
        {"Keybinds [D F J K]", true, true}
    };

    selectedIndex = 0;
    currentMenu = MenuState::MAIN;

    running = true;
    return true;
}
