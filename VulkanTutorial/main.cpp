#include <stdexcept>

#include "hello_triangle.h"

int main() {
    HelloTriangleApplication app;

    try {
        cout << "app run" << endl;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}