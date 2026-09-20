#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#if defined(__INTELLISENSE__) || !define(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainloop();
		cleanup();
	}
private:
	GLFWwindow* window = nullptr;
	vk::raii::Context context;
	vk::raii::Instance instance = nullptr;

	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}
	void initVulkan()
	{
		createInstance();
	}
	void createInstance()
	{
		constexpr vk::ApplicationInfo appInfo
		{
			.pApplicationName		= "Hello Triangle",
			.applicationVersion		= VK_MAKE_VERSION(1,0,0),
			.pEngineName			= "No Engine",
			.engineVersion			= VK_MAKE_VERSION(1,0,0),
			.apiVersion				= vk::ApiVersion14
		};
		uint32_t	glfwExtensionCount	= 0;
		auto		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		
		auto extensionProperties = context.enumerateInstanceExtensionProperties();
		for (uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			if (std::ranges::none_of(extensionProperties,
				[glfwExtension = glfwExtensions[i]](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
			{
				throw std::runtime_error("Required GLFW extension not supported: " + std::string(glfwExtensions[i]));
			}
		}

		vk::InstanceCreateInfo createInfo
		{
			.pApplicationInfo			= &appInfo,
			.enabledExtensionCount		= glfwExtensionCount,
			.ppEnabledExtensionNames	= glfwExtensions
		};
		instance = vk::raii::Instance(context, createInfo);
	}
	void mainloop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
	void cleanup()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

};

int main()
{
	try
	{
		HelloTriangleApplication app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_FAILURE;

	VkInstance instance;
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = 0;
	createInfo.ppEnabledExtensionNames = nullptr;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;
	
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create instance!");
	}
	vkDestroyInstance(instance, nullptr);

}
	