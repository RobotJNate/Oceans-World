LRESULT CALLBACK OW_Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OW_Window* window = reinterpret_cast<OW_Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_CREATE:
    {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        break;
    }
    case WM_KEYDOWN:
        if (window)
            window->keyStates[wParam] = true;
        break;
    case WM_KEYUP:
        if (window)
            window->keyStates[wParam] = false;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
