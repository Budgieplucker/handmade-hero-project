#include <windows.h>

#define local_persist       static
#define global_variable     static 
#define internal            static 

global_variable bool Running;

typedef struct {
    BITMAPINFO BitmapInfo;
    void* BitmapMemory;
    HBITMAP BitmapHandle;
    HDC BitmapDeviceContext;
    int BitmapWidth;
    int BitmapHeight;
} OffscreenBuffer;

global_variable OffscreenBuffer GlobalOffscreenBuffer;

internal void
Win32ResizeDIBSection(OffscreenBuffer* Buffer, int Width, int Height)
{
    if(Buffer->BitmapHandle)
    {
        DeleteObject(Buffer->BitmapHandle);
    }
    if(!Buffer->BitmapDeviceContext)
    {
        Buffer->BitmapDeviceContext = CreateCompatibleDC(0);
    }

    Buffer->BitmapInfo.bmiHeader.biSize = sizeof(Buffer->BitmapInfo.bmiHeader);
    Buffer->BitmapInfo.bmiHeader.biWidth = Width;
    Buffer->BitmapInfo.bmiHeader.biHeight = Height;
    Buffer->BitmapInfo.bmiHeader.biPlanes = 1;
    Buffer->BitmapInfo.bmiHeader.biBitCount = 32;
    Buffer->BitmapInfo.bmiHeader.biCompression = BI_RGB;
    Buffer->BitmapInfo.bmiHeader.biSizeImage = 0;
    Buffer->BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    Buffer->BitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    Buffer->BitmapInfo.bmiHeader.biClrUsed = 0;
    Buffer->BitmapInfo.bmiHeader.biClrImportant = 0;

    Buffer->BitmapDeviceContext = CreateCompatibleDC(0);

    Buffer->BitmapHandle = CreateDIBSection (
        Buffer->BitmapDeviceContext, &Buffer->BitmapInfo, DIB_RGB_COLORS,
        &Buffer->BitmapMemory,
        0, 0
    );
}

internal void
Win32UpdateWindow(HDC DeviceContext, OffscreenBuffer Buffer, int X, int Y, int Width, int Height)
{
    StretchDIBits (
        DeviceContext, X, Y, Width, Height,
        X, Y, Width, Height,
        Buffer.BitmapMemory,
        &Buffer.BitmapInfo,
        DIB_RGB_COLORS, SRCCOPY
    );
}

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
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;
            Win32ResizeDIBSection(&GlobalOffscreenBuffer, Width, Height);
            OutputDebugStringA("WM_SIZE\n");
        } break;
        
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            Running = false;
        } break;

        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            Running = false;
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

            Win32ResizeDIBSection(&GlobalOffscreenBuffer, Width, Height);
            Win32UpdateWindow(DeviceContext, GlobalOffscreenBuffer, X, Y, Width, Height);

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
            Running = true;
            MSG Message;
            while(Running)
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