

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(cgif REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

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
    WinBox.h
    ChooseMode.h
    modeBorder.h
    Background.h
    app.o
)

if (WIN32)
    target_link_options(app PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
else()
    target_link_options(app PRIVATE "-mwindows")
endif()

target_link_libraries(app PRIVATE
    glad::glad
    glm::glm
    glfw
    cgif::cgif
)









