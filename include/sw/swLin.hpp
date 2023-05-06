#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>

void keyPressed(char key) {
    // handle key press
}

int main() {
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        // handle error
        return 1;
    }

    int screen = DefaultScreen(display);
    int width = 640;
    int height = 480;
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, width, height, 0, 0, 0);

    XSetStandardProperties(display, window, "Window Title", "Icon Title", None, nullptr, 0, nullptr);
    XSelectInput(display, window, StructureNotifyMask | ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    XEvent event;
    do {
        XNextEvent(display, &event);
    } while (event.type != MapNotify || event.xmap.event != window);

    uint32_t *framebuffer = nullptr;
    XImage *image = XCreateImage(display, DefaultVisual(display, screen), 32, ZPixmap, 0, (char *) framebuffer, width, height, 32, 0);

    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, BlackPixel(display, screen));
    XSetBackground(display, gc, WhitePixel(display, screen));

    while (true) {
        XNextEvent(display, &event);

        if (event.type == KeyPress) {
            char buffer[1];
            KeySym keysym;
            XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr);
            if (buffer[0] != '\0') {
                keyPressed(buffer[0]);
            }
        }
        // handle other event types, such as expose events
    }

    XDestroyImage(image);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}