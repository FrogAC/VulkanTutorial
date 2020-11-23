#include <stdexcept>
#include <iostream>
#include "vulkan_app.h"

class HelloTriangleApplication : public VulkanApp {
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
