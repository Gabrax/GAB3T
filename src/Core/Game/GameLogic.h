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
#define NOMINMAX
#include <enet/enet.h>


constexpr float changeX = 2.1f;
constexpr float changeY = 2.1f;
constexpr int BOARD_SIZE = 4;
static bool isEnd = false, PvPmode = false, PvEmode = false;
static char Pwins, Ewins;
static int score;

static std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> board;

enum GameState {
    MENU,
    PVP_MODE,
    PVE_MODE,
    MULTI_MODE
};

static GameState currentState = MENU;

#define INIT_TEXT(name) GLTtext* name = gltCreateText()
    
struct Game {

    Game();
    void Update();

    void printTuple();
    void printBoard(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
        
private:
    float orbitRadius = 5.0f; 
    float orbitSpeed = 1.0f;  

    float time, x, y, red, green, blue; 
    glm::vec3 lightPosition = glm::vec3(0.0f);
    glm::vec4 gradientColor = glm::vec4(0.0f);
    glm::vec3 selectPos = glm::vec3(0.0f);

    INIT_TEXT(title);
    INIT_TEXT(Owins);
    INIT_TEXT(Xwins);
    INIT_TEXT(Draw);
    INIT_TEXT(PvsAI);
    INIT_TEXT(Local);
    INIT_TEXT(Multiplayer);
    INIT_TEXT(Host);
    INIT_TEXT(Client);
    INIT_TEXT(Connected);
    INIT_TEXT(Waiting);
    INIT_TEXT(WaitingHost);
    INIT_TEXT(WaitingClient);
    INIT_TEXT(Disconnected);
    INIT_TEXT(HostWins);
    INIT_TEXT(ClientWins);

    std::vector<std::tuple<char,float,float>> check;
    std::array<std::pair<float, float>, BOARD_SIZE * BOARD_SIZE> mapCoord = {
        std::make_pair(-2.1f, 2.1f),  std::make_pair(0, 2.1f),  std::make_pair(2.1f, 2.1f),  std::make_pair(4.2f, 2.1f),
        std::make_pair(-2.1f, 0),     std::make_pair(0, 0),     std::make_pair(2.1f, 0),     std::make_pair(4.2f, 0),
        std::make_pair(-2.1f, -2.1f), std::make_pair(0, -2.1f), std::make_pair(2.1f, -2.1f), std::make_pair(4.2f, -2.1f),
        std::make_pair(-2.1f, -4.2f), std::make_pair(0, -4.2f), std::make_pair(2.1f, -4.2f), std::make_pair(4.2f, -4.2f)
    };

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
    // MENU INPUT //     
    void handleBorderInput();
    void ReturnToMenu();
    // MENU INPUT //
    // PVP MODE //
    void PVPhandlePlayersInput();
    void handlePlayerInput();
    void handleEnemyInput();
    // PVP MODE //
    void HandlePlayerMoving();
    // PVE MODE //
    void PVEhandlePlayersInput();
    void handleAiInput();
    int minimax(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, int depth, bool maximizingPlayer = false,int alpha = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max());
    std::vector<std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>> generateMoves(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, bool maximizingPlayer);
    int evaluate(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    bool gameIsOver(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    // PVE MODE // 

    // MULTIPLAYER //
    void SendMoveToClient(int row, int col, char player);
    void UpdateClient();
    void SendMoveToHost(int row, int col, char player);
    void MULTIhandlePlayersInput();
    void UpdateHost();
    void SendRESET();
    // MULTIPLAYER //

    // BOARD LOGIC //
    void handleAnimation();
    void ResetGame();
    bool PositionTaken(float x, float y);
    void updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y);
    unsigned int countPlayers(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> createEmptyBoard();
    inline void ClearBoard() { for(auto& i : board) i.fill(' '); }
    char checkWinner(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player);
    // BOARD LOGIC //
};
