#include <opencv2/opencv.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

#include <unistd.h>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <vector>

void drawWindows(Display* display, int screen, std::vector<cv::Mat>& images) {
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                        0, 0, images[0].cols, images[0].rows, 0, 0, WhitePixel(display, screen));

    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, nullptr);

    for (int image = 0; image < images.size(); image++) {
        for (int y = 0; y < images[image].rows; ++y) {
            for (int x = 0; x < images[image].cols; ++x) {
                if (images[image].at<unsigned char>(y, x) > 0) {
                    XDrawPoint(display, window, gc, x, y);
                }
            }
        }

        usleep(1000000 / 30);
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

    int count = 0;

    std::vector<cv::Mat> images;

    std::cout << "Reading images..." << std::endl;

    std::filesystem::directory_iterator it("./badapple");

    for (const auto& entry : it) {
        count++;
    }

    for (int i = 1; i <= count; i++) {
        std::string filename = "output_" + std::to_string(i);
        filename = "./badapple/output_" + std::string(4 - std::to_string(i).length(), '0') + std::to_string(i) + ".png";
        cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

        if (image.empty()) {
            std::cerr << "Failed to read image." << std::endl;
            return 1;
        }

        images.push_back(image);
    }

    std::cout << "Images read! Launching window..." << std::endl;

    drawWindows(display, screen, images);

    return 0;
}
