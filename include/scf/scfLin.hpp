#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <functional>
#include <thread>
#include <stdint.h>
#include <iostream>
#include "../dsl/ctx.hpp"
#include "../dsl/mutex.hpp"

struct vec2D
{
    uint32_t x;
    uint32_t y;
};


typedef dsl::ctxTemplate<dsl::argb8888> ctx8888;

    class scf{
            Lock loop_mutex;
            uint32_t width;
            uint32_t height;
            Window child;//this is stupid
            Display* display;
            XVisualInfo vinfo;
            XSetWindowAttributes attr;
            Window root;
            Window win;
            GC gc;
            Atom wm_delete_window;
            uint8_t screenW;
            uint8_t screenH;
        public:
            scf(uint32_t w,uint32_t h,std::function<void(scf&,ctx8888&,int32_t,int32_t)> f){
                width = w;
                height = h;
                
                display = XOpenDisplay(NULL);

                screenW = XDisplayWidth(display, DefaultScreen(display));
                screenH = XDisplayHeight(display, DefaultScreen(display));

                XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

                attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
                attr.border_pixel = 0;
                attr.background_pixel = 0;
                attr.override_redirect = True; // Set the override_redirect attribute to True

                root = DefaultRootWindow(display);
                win = XCreateWindow(display, root, 0, 0, width, height, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr); // Add CWOverrideRedirect to the value mask

                XSelectInput(display, win, StructureNotifyMask | ButtonPressMask);
                gc = XCreateGC(display, win, 0, 0);

                wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
                XSetWMProtocols(display, win, &wm_delete_window, 1);

                XMapWindow(display, win);

                int keep_running = 1;
                XEvent event;

                ctx8888 ctx(width, height);
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

                

                while (keep_running) {
                    WriteLock lock(loop_mutex);
                    XQueryPointer(display, root, &root, &child, &root_x, &root_y, &win_x, &win_y, &mask);
                    
                    f(*this,ctx,root_x,root_y);
                    
                    XFlush(display);
                    XPutImage(display, win, gc, img, 0, 0, 0, 0, width, height);
                    std::this_thread::sleep_for(std::chrono::milliseconds(16));
                }
            }
            void move(int32_t x,int32_t y){
                XMoveWindow(display, win, x, y);
            }
            vec2D screenSize(){
                return vec2D{screenW,screenH};
            }

    };
