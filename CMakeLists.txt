cmake_minimum_required(VERSION 3.15)

project(VKTest)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

include_directories(
  external/glfw/include
  external/glm
  /VulkanSDK/1.4.335.0/Include/
)

add_executable(main
  main.cpp
)