

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)



add_executable(app 
    main.cpp 
    engine.h 
    engine.cpp
    window.h
    window.cpp
    camera.h
    Shader.h
    stb_image.cpp
    stb_image.h
)

target_sources(app PRIVATE 
    ../Shaders/Crosshair.vert 
    ../Shaders/Crosshair.frag
)

target_link_libraries(app PRIVATE
    glad::glad
    glm::glm
    glfw
)









