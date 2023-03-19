#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <thread>
#include <stdint.h>
#include <iostream>
#include "ctx.hpp"
#include "mutex.hpp"

Lock loop_mutex;

int main(int argc, char* argv[]){
    uint32_t width = 64;
    uint32_t height = 64;
    Display* display = XOpenDisplay(NULL);

    XVisualInfo vinfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0;
    attr.override_redirect = True; // Set the override_redirect attribute to True

    Window root = DefaultRootWindow(display);
    Window win = XCreateWindow(display, root, 0, 0, width, height, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr); // Add CWOverrideRedirect to the value mask

    XSelectInput(display, win, StructureNotifyMask | ButtonPressMask);
    GC gc = XCreateGC(display, win, 0, 0);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, win, &wm_delete_window, 1);

    XMapWindow(display, win);

    int keep_running = 1;
    XEvent event;

    pina::ctxTemplate<uint32_t> ctx(width, height);
    ctx.fillCircle(32,32,32,pina::RGB888(255,255,255));
    XImage* img = XCreateImage(display, vinfo.visual, 32, ZPixmap, 0, (char *)ctx.img, width, height, 8, 0);
    
    int root_x, root_y, win_x, win_y;
    unsigned int mask;
    
    std::thread([&](){
        WriteLock lock(loop_mutex);
        XNextEvent(display, &event);

        switch(event.type) {
            case ClientMessage:
                if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", 1) && (Atom)event.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", 1))
                    keep_running = 0;

                break;
            default:
                break;
        }
    }).detach();

    Window child;//this is stupid

    while (keep_running) {
        WriteLock lock(loop_mutex);
        XMoveWindow(display, win, 0, 0);
        XQueryPointer(display, root, &root, &child, &root_x, &root_y, &win_x, &win_y, &mask);
        
        ctx.fillCircle(32,32,32,pina::RGB888(root_y,0,0));
        XFlush(display);
        XPutImage(display, win, gc, img, 0, 0, 0, 0, width, height);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
