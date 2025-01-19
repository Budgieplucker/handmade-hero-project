#include <windows.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    MessageBoxA(0, "Handmade", "Handmade Hero", MB_OK|MB_ICONINFORMATION);
    return (0);
}