

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)


add_executable(SRC main.cpp)
target_link_libraries(SRC PRIVATE
    glad::glad
    glfw
)



