#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "vulkan_app.h"

using namespace std;

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) func(instance, debugMessenger, pAllocator);
}

VulkanApp::VulkanApp(uint32_t width, uint32_t height, string appName) {
	glfwWindow = initWindow(width, height, appName);
	instance = initVkInstance(kValidationLayers, appName);
	if (DEBUG) debugMessenger = initVkDebugMessenger(instance);
}

VulkanApp::~VulkanApp() {
	cout << "Cleaning up ..." << endl;
	DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

GLFWwindow* VulkanApp::initWindow(uint32_t width, uint32_t height, string winName) {
	glfwInit();
	// no GL, no resize
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	return glfwCreateWindow(width, height, winName.c_str(), nullptr, nullptr);
}

VkInstance VulkanApp::initVkInstance(const STRINGVEC_T& validLayers, string appName) {
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Default Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	// EXTENSIONS
	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 0;
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (DEBUG) {
		// VK VALIDATION
		if (!checkValidationLayers(validLayers)) throw runtime_error("Layer not found");
		createInfo.enabledLayerCount = static_cast<uint32_t>(validLayers.size());
		createInfo.ppEnabledLayerNames = validLayers.data();
		// VK DEBUG MESSENGER INFO
		debugCreateInfo = getVkDebugMessenger();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	VkInstance instance;
	VkResult r = vkCreateInstance(&createInfo, nullptr, &instance);
	if (r != VK_SUCCESS) throw runtime_error("Fail create instance");
	return move(instance);
}

VkDebugUtilsMessengerEXT VulkanApp::initVkDebugMessenger(VkInstance instance) {
	VkDebugUtilsMessengerCreateInfoEXT createInfo = getVkDebugMessenger();
	VkDebugUtilsMessengerEXT debugMessenger;
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Fail set debug messenger");
	}
	return move(debugMessenger);
}

VkDebugUtilsMessengerCreateInfoEXT VulkanApp::getVkDebugMessenger() {
	VkDebugUtilsMessengerCreateInfoEXT info{};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info.pfnUserCallback = debugCallback;
	return move(info);
}

STRINGVEC_T VulkanApp::getRequiredExtensions() {
	// GLFW EXTENSIONS
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	STRINGVEC_T exts(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (DEBUG) {
		exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);  // VK_EXT_debug_utils
	}
	cout << "Required Extensions:" << endl;
	for (const auto e : exts) cout << "\t" << e << endl;
	return move(exts);
}

bool VulkanApp::checkValidationLayers(const STRINGVEC_T& validLayers) {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	cout << "Available Validation Layers :" << endl;
	for (const auto& l : availableLayers) cout << "\t" << l.layerName << endl;
	cout << "Requested Validation Layers :" << endl;
	for (const char* l : validLayers) cout << "\t" << l << endl;

	for (const char* layer : validLayers) {
		bool found = false;
		for (const auto& av : availableLayers)
			if (strcmp(layer, av.layerName) == 0) {
				found = true;
				break;
			}
		if (!found) return false;
	}
	return true;
}

void VulkanApp::run() {
	while (!glfwWindowShouldClose(glfwWindow)) {
		glfwPollEvents();
		mainLoop();
	}
}
