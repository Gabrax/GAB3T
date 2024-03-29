

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE
glad::glad
glfw
)



