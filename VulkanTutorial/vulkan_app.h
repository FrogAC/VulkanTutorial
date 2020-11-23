/*
	A vulkan application represent handles debug and pipeline setup
*/
#pragma once
#define GLFW_INCLUDE_VULKAN 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#ifdef NDEBUG
const bool DEBUG = false;
#else
const bool DEBUG = true;
#endif

using STRINGVEC_T = std::vector<const char*>;

class VulkanApp {
public:
	VulkanApp() : VulkanApp(800, 600, "Vulkan") {};
	VulkanApp(uint32_t width, uint32_t height, std::string appName);
	~VulkanApp();
	void run();
private:
	const STRINGVEC_T kValidationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
	GLFWwindow* glfwWindow;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	void mainLoop() {}

	static GLFWwindow* initWindow(uint32_t width, uint32_t height, std::string winName);
	static VkInstance initVkInstance(const STRINGVEC_T& validLayers, std::string appName);
	static VkDebugUtilsMessengerEXT initVkDebugMessenger(VkInstance instance);
	static STRINGVEC_T getRequiredExtensions();
	static VkDebugUtilsMessengerCreateInfoEXT getVkDebugMessenger();
	static bool checkValidationLayers(const STRINGVEC_T& validLayers);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};
