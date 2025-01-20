#include <windows.h>

WNDPROC Wndproc;

LRESULT CALLBACK MainWindowCallback(
  HWND Window,
  UINT Message,
  WPARAM wParam,
  LPARAM lParam)
{
    LRESULT Result = 0;

    switch(Message) 
    {
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
        } break;
        
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
        } break;
        
        case WM_ACTIVATEAPP:
        {  
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

            static DWORD Operation = WHITENESS;
            PatBlt(DeviceContext, X, Y, Width, Height, Operation);

            if(Operation == WHITENESS)
            {
                Operation = BLACKNESS;
            }
            else
            {
                Operation = WHITENESS;
            }

            EndPaint(Window, &Paint);
        } break;

        default:
        {
            // OutputDebugStringA("WM_DEFAULT\n");
            Result = DefWindowProc(Window, Message, wParam, lParam);
        } break;
    }

    return Result;
}

int CALLBACK WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR     CommandLine,
    int       ShowCode
)
{
    WNDCLASSA WindowClass = {};

    WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance; // Can also use the function "GetModuleInstance(0)" with zero passed as parameter to get the currently running instance from the kernel.
    WindowClass.lpszClassName = "Handmade Hero Window Class";

    if(RegisterClassA(&WindowClass))
    {   
        HWND WindowHandle = CreateWindowExA
        (
            0,
            WindowClass.lpszClassName,
            "Handmade Hero",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            WindowClass.hInstance, 
            0
        );

        if(WindowHandle != NULL)
        {
            MSG Message;
            for(;;)
            {
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0);

                if(MessageResult > 0)
                {
                    TranslateMessage(&Message);     
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
        }
    }

    return (0);
}