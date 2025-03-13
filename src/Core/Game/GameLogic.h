#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <tuple>
#include <cmath>

#include "../../Input/Input.h"
#include "Players.h"
#include "../Backend/window.h"

#include "../Backend/LightManager.h"
#include "../Backend/EnvironmentMap.h"
#include "../Backend/StaticModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include "gltext.h"
#include <chrono>


constexpr float changeX = 2.1f;
constexpr float changeY = 2.1f;
constexpr int BOARD_SIZE = 3;
static bool isEnd = false;
static bool PvPmode = false;
static bool PvEmode = false;
static char Pwins;
static char Ewins;
static int score;

static std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> board;

enum GameState {
    MENU,
    PVP_MODE,
    PVE_MODE,
    MULTI_MODE
};

static GameState currentState = MENU;
    
struct Game {

    Game();
    void Update();

    void printTuple();
    void printBoard(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
        
private:
    float orbitRadius = 5.0f; 
    float orbitSpeed = 1.0f;  

    float time;
    float x;
    float y; 
    glm::vec3 lightPosition = glm::vec3(0.0f);
    float red;  
    float green;
    float blue;
    glm::vec4 gradientColor = glm::vec4(0.0f);

    GLTtext* title = gltCreateText();
    GLTtext* Owins = gltCreateText();
    GLTtext* Xwins = gltCreateText();
    GLTtext* Draw = gltCreateText();
    GLTtext* PvsAI = gltCreateText();
    GLTtext* Local = gltCreateText();
    GLTtext* Multiplayer = gltCreateText();
    GLTtext* Host = gltCreateText();
    GLTtext* Client = gltCreateText();
    GLTtext* Connected = gltCreateText();
    GLTtext* Disconnected = gltCreateText();
    GLTtext* Host_Adress = gltCreateText();

    std::vector<std::tuple<char,float,float>> check;
    std::array<std::pair<float, float>, 9> mapCoord = {
        std::make_pair(-2.1f, 2.1f), std::make_pair(0, 2.1f), std::make_pair(2.1f, 2.1f),
        std::make_pair(-2.1f, 0), std::make_pair(0, 0), std::make_pair(2.1f, 0),
        std::make_pair(-2.1f, -2.1f), std::make_pair(0, -2.1f), std::make_pair(2.1f, -2.1f)
    };

    glm::vec3 selectPos = glm::vec3(0.0f);

    std::shared_ptr<StaticModel> circleModel = std::make_shared<StaticModel>("res/models/Circle.obj");
    std::vector<Circle> circles;
    std::shared_ptr<StaticModel> crossModel = std::make_shared<StaticModel>("res/models/Cross.obj");
    std::vector<Cross> crosses;
    std::shared_ptr<StaticModel> boardModel = std::make_shared<StaticModel>("res/models/Map.obj");
    std::shared_ptr<StaticModel> selectModel = std::make_shared<StaticModel>("res/models/Select.obj");
    LightManager manager;
    EnvironmentMap envMap;
    bool isPlayerTurn;
    bool isAnimating = false;
    float animationZ = 2.5f;  // Starting Z position for animation
    const float animationSpeed = 0.05f;  // Speed of sliding animation
    std::chrono::time_point<std::chrono::steady_clock> animationStart;
    void handleAnimation();
    // MENU INPUT //     
    void handleBorderInput();
    void ReturnToMenu();
    // MENU INPUT //
    void HandlePlayerMoving();
    // PVP MODE //
    void PVPhandlePlayersInput();
    void handlePlayerInput();
    void handleEnemyInput();
    // PVP MODE //

    // PVE MODE //
    void PVEhandlePlayersInput();
    void handleAiInput();
    int minimax(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, int depth, bool maximizingPlayer);
    std::vector<std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>> generateMoves(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player);
    int evaluate(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    bool gameIsOver(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    // PVE MODE // 

    // BOARD LOGIC //
    void ResetGame();
    bool PositionTaken(float x, float y);
    void updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y);
    unsigned int countPlayers(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> createEmptyBoard();
    void ClearBoard();
    char checkifPwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    char checkifEwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    // BOARD LOGIC //
};
