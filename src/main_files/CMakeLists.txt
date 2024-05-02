

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
    Utility.h
    stb_image.cpp
    stb_image.h
    Input.cpp
    Input.h
    ControllerASCII.h
    GameMap.h
    Title.h
    Select.h
    Player.h
    Enemy.h
    GameLogic.h
)


target_link_libraries(app PRIVATE
    glad::glad
    glm::glm
    glfw
)









