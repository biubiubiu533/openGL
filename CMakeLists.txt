cmake_minimum_required(VERSION 3.24)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

#glad
include_directories(./glad/include)

#glfw
include_directories(/opt/homebrew/Cellar/glfw/3.3.8/include)
link_directories(/opt/homebrew/Cellar/glfw/3.3.8/lib)

#glm
include_directories(/opt/homebrew/Cellar/glm/0.9.9.8/include)
#link_directories(/opt/homebrew/Cellar/glm/0.9.9.8/lib)


add_executable(LearnOpenGL
        ./glad/src/glad.c
        main.cpp camera_class.h mesh.h model.h)

target_link_libraries(
        LearnOpenGL
        glfw
)

find_package(assimp REQUIRED) # 添加assimp
target_link_libraries(LearnOpenGL assimp::assimp)
