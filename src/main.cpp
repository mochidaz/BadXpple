#include <X11/Xlib.h>

#include <iostream>
#include <vector>
#include <thread>

#include "audio.cpp"
#include "pixmap.cpp"

//for bigger scale
//void draw_pixels(Display* display, int screen, std::vector<bxpl::Pixmap>& images) {
//    int screen_width = DisplayWidth(display, screen);
//    int screen_height = DisplayHeight(display, screen);
//
//    int window_width = images[0].w * 2;
//    int window_height = images[0].h * 2;
//
//    int x = (screen_width - window_width) / 2;
//    int y = (screen_height - window_height) / 2;
//
//    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
//                                        x, y, window_width, window_height, 0, 0, BlackPixel(display, screen));
//    XMapWindow(display, window);
//
//    GC gc = XCreateGC(display, window, 0, nullptr);
//
//    XSetWindowAttributes attributes;
//    attributes.override_redirect = false;
//    XChangeWindowAttributes(display, window, CWOverrideRedirect, &attributes);
//
//    XSetForeground(display, gc, WhitePixel(display, screen));
//
//    int delayMicroseconds = 10000000 / 330;
//
//    for (int image = 0; image < images.size(); image++) {
//        for (int y = 0; y < images[image].h; ++y) {
//            for (int x = 0; x < images[image].w; ++x) {
//                int index = y * images[image].w + x;
//                // black pixel range is 0-127
//                if ((images[image].data[index].r + images[image].data[index].g + images[image].data[index].b) / 3 > 127) {
//                    XDrawPoint(display, window, gc, x * 2, y * 2);
//                    XDrawPoint(display, window, gc, x * 2 + 1, y * 2);
//                    XDrawPoint(display, window, gc, x * 2, y * 2 + 1);
//                    XDrawPoint(display, window, gc, x * 2 + 1, y * 2 + 1);
//                }
//            }
//        }
//
//        std::this_thread::sleep_for(std::chrono::microseconds(delayMicroseconds));
//        XClearWindow(display, window);
//    }
//}

void draw_pixels(Display* display, int screen, std::vector<bxpl::Pixmap>& images) {
    int screen_width = DisplayWidth(display, screen);
    int screen_height = DisplayHeight(display, screen);

    int window_width = images[0].w;
    int window_height = images[0].h;

    int x = (screen_width - window_width) / 2;
    int y = (screen_height - window_height) / 2;

    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                        x, y, window_width, window_height, 0, 0, BlackPixel(display, screen));
    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, nullptr);

    XSetWindowAttributes attributes;
    attributes.override_redirect = false;
    XChangeWindowAttributes(display, window, CWOverrideRedirect, &attributes);

    XSetForeground(display, gc, WhitePixel(display, screen));

    int delay_microseconds = 10000000 / 330;

    for (int image = 0; image < images.size(); image++) {
        for (int y = 0; y < images[image].h; ++y) {
            for (int x = 0; x < images[image].w; ++x) {
                // black pixel range is 0-127
                if (images[image].at(x, y) > 127) {
                    XDrawPoint(display, window, gc, x, y);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds(delay_microseconds));
        XClearWindow(display, window);
    }
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error opening display." << std::endl;
        return 1;
    }

    int screen = DefaultScreen(display);

    std::vector<bxpl::Pixmap> pixmaps = bxpl::read_bin("./resources/badapple.bin");

    std::cout << "Reading images..." << std::endl;

    std::cout << "Images read! Launching window..." << std::endl;

    std::thread audio_thread(play_audio, "./resources/badapple.wav");

    draw_pixels(display, screen, pixmaps);

    audio_thread.native_handle();

    XFlush(display);
    XCloseDisplay(display);

    return 0;
}
