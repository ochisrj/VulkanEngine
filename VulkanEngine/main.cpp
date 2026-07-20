#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <vector>
#include <stdexcept>
#include <iostream>

static VkInstance         g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice   g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice           g_Device = VK_NULL_HANDLE;
static uint32_t           g_QueueFamily = (uint32_t)-1;
static VkQueue             g_Queue = VK_NULL_HANDLE;
static VkDescriptorPool   g_DescriptorPool = VK_NULL_HANDLE;

void CreateInstance(GLFWwindow* window)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanEngine";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t extCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &g_Instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Vulkan instance");
}

void PickPhysicalDeviceAndQueue()
{
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(g_Instance, &gpuCount, nullptr);
    std::vector<VkPhysicalDevice> gpus(gpuCount);
    vkEnumeratePhysicalDevices(g_Instance, &gpuCount, gpus.data());
    g_PhysicalDevice = gpus[0]; // pick first GPU for simplicity

    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> queues(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &queueCount, queues.data());

    for (uint32_t i = 0; i < queueCount; i++)
    {
        if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            g_QueueFamily = i;
            break;
        }
    }
}

void CreateLogicalDevice()
{
    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = g_QueueFamily;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    const char* deviceExtensions[] = { "VK_KHR_swapchain" };

    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(g_PhysicalDevice, &deviceInfo, nullptr, &g_Device) != VK_SUCCESS)
        throw std::runtime_error("Failed to create logical device");

    vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
}

void CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSizes[] = { { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 64 } };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 64;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = poolSizes;

    vkCreateDescriptorPool(g_Device, &poolInfo, nullptr, &g_DescriptorPool);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vulkan Engine + ImGui", nullptr, nullptr);

    CreateInstance(window);
    PickPhysicalDeviceAndQueue();
    CreateLogicalDevice();
    CreateDescriptorPool();

    // --- ImGui setup ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo initInfo{};
    initInfo.Instance = g_Instance;
    initInfo.PhysicalDevice = g_PhysicalDevice;
    initInfo.Device = g_Device;
    initInfo.QueueFamily = g_QueueFamily;
    initInfo.Queue = g_Queue;
    initInfo.DescriptorPool = g_DescriptorPool;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = 2;
    /*initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;*/
    // NOTE: You must also set initInfo.RenderPass (or use dynamic rendering)
    // once you've built your swapchain + render pass. Left out here for brevity.
    ImGui_ImplVulkan_Init(&initInfo);

    std::cout << "Vulkan instance, device, and ImGui context created successfully.\n";

    // --- Main loop (window handling only; add your render pass + swapchain to draw) ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Tst");
        ImGui::Text("aads");
        ImGui::End();

        ImGui::ShowDemoWindow();
        ImGui::Render();

    }

    // --- Cleanup ---
    vkDeviceWaitIdle(g_Device);
     ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(g_Device, g_DescriptorPool, nullptr);
    vkDestroyDevice(g_Device, nullptr);
    vkDestroyInstance(g_Instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}