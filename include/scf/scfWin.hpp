#include <Windows.h>
#include <functional>
#include <thread>
#include <cstdint>
#include <iostream>

typedef uint32_t RGB8888;

class scf {
public:
    scf(uint32_t w, uint32_t h, std::function<void(scf&, RGB8888*, int32_t, int32_t)> f) {
        width = w;
        height = h;

        WNDCLASS wc = {0};
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProc;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = "BorderlessWindowClass";
        RegisterClass(&wc);

        DWORD dwStyle = WS_POPUP | WS_VISIBLE;
        HWND hWnd = CreateWindow(wc.lpszClassName, NULL, dwStyle, 0, 0, width, height, NULL, NULL, NULL, NULL);
        SetWindowLongPtr(hWnd, GWL_STYLE, 0);

        RGB8888* buffer = new RGB8888[width * height];

        int keep_running = 1;
        MSG msg;

        while (keep_running) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    keep_running = 0;
                }
                else {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else {
                f(*this, buffer, 0, 0);
                HDC hdc = GetDC(hWnd);
                BitBlt(hdc, 0, 0, width, height, NULL, NULL, NULL, BLACKNESS);
                SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, buffer, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
                ReleaseDC(hWnd, hdc);
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        }

        delete[] buffer;
    }

    void move(int32_t x, int32_t y) {
        HWND hWnd = FindWindow("BorderlessWindowClass", NULL);
        SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

private:
    uint32_t width;
    uint32_t height;
    BITMAPINFO bmi;
};