
#include <iostream>
#include <optional>
#include <set>

#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>



#pragma comment(lib, "vulkan-1.lib")



VkInstance initVulkan(VkDevice *deviceOut, VkPhysicalDevice *physdev) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Poop Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  // vkCreateInstance()

  VkInstanceCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
createInfo.pApplicationInfo = &appInfo;

uint32_t glfwExtensionCount = 0;
const char** glfwExtensions;

glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

createInfo.enabledExtensionCount = glfwExtensionCount;
createInfo.ppEnabledExtensionNames = glfwExtensions;

createInfo.enabledLayerCount = 0;

VkInstance instance;
VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
}

VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

uint32_t deviceCount = 0;
vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
}

std::vector<VkPhysicalDevice> devices(deviceCount);
vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

physicalDevice = devices.at(0);

*physdev = physicalDevice;

uint32_t queueFamilyCount = 0;
vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

int i = 0;
for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        // indices.graphicsFamily = i;
        break;
    }

    i++;
}

VkDeviceQueueCreateInfo queueCreateInfo{};
queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
queueCreateInfo.queueFamilyIndex = i;
queueCreateInfo.queueCount = 1;

float queuePriority = 1.0f;
queueCreateInfo.pQueuePriorities = &queuePriority;

VkPhysicalDeviceFeatures deviceFeatures{};

VkDeviceCreateInfo deviceCreateInfo{};
deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

deviceCreateInfo.enabledExtensionCount = 0;

  deviceCreateInfo.enabledLayerCount = 0;

  VkDevice device;

  if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
}

VkQueue graphicsQueue;

vkGetDeviceQueue(device, queueCreateInfo.queueFamilyIndex, 0, &graphicsQueue);



 *deviceOut = device;

return instance;
}

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

// struct QueueFamilyIndices {
//     uint32_t graphicsFamily;
// };


bool isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    // Logic to find queue family indices to populate struct with

    std::optional<uint32_t> graphicsFamily;

std::cout << std::boolalpha << graphicsFamily.has_value() << std::endl; // false

graphicsFamily = 0;

std::cout << std::boolalpha << graphicsFamily.has_value() << std::endl;

uint32_t queueFamilyCount = 0;
vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
int i = 0;
for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphicsFamily = i;
    }

    if (indices.isComplete()) {
        break;
    }

    i++;
}

VkBool32 presentSupport = false;
vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport) {
    indices.presentFamily = i;
}
    return indices;
}
VkSurfaceKHR surface;

int main() {
  std::cout << "hi";

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::cout << extensionCount << " extensions supported\n";

  glm::mat4 matrix;
  glm::vec4 vec;
  auto test = matrix * vec;

  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkInstance instance = initVulkan(&device, &physicalDevice);


VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
}

if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    VkBool32 presentSupport = false;
vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &presentSupport);

// if (presentSupport) {
//     indices.presentFamily = i;
// }

VkQueue presentQueue;


QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

float queuePriority = 1.0f;
for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
}

  while(!glfwWindowShouldClose(window)) {
      glfwPollEvents();
  }

  vkDestroySurfaceKHR(instance, surface, nullptr);
    // vkDestroyInstance(instance, nullptr);

  vkDestroyInstance(instance, nullptr);

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}