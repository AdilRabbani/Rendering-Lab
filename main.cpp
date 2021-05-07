#include <iostream>

int main() {

    // Image

    const int image_width = 3;
    const int image_height = 3;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    std::cout << 255 << ' ' << 0 << ' ' << 0 << '\n';
    std::cout << 0 << ' ' << 255 << ' ' << 0 << '\n';
    std::cout << 0 << ' ' << 0 << ' ' << 0 << '\n';
    std::cout << 0 << ' ' << 0 << ' ' << 255 << '\n';
    std::cout << 255 << ' ' << 255 << ' ' << 0 << '\n';
    std::cout << 255 << ' ' << 0 << ' ' << 255 << '\n';
    std::cout << 255 << ' ' << 255 << ' ' << 255 << '\n';
    std::cout << 0 << ' ' << 255 << ' ' << 255 << '\n';
    std::cout << 255 << ' ' << 100 << ' ' << 0 << '\n';
}