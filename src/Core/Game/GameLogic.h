#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <tuple>
#include <cmath>

#include "../../Input/Input.h"
#include "GameMap.h"
#include "Select.h"
#include "Players.h"
#include "ChooseMode.h"
#include "modeBorder.h"
#include "Background.h"
#include "../Backend/window.h"

#include "../Backend/LightManager.h"
#include "../Backend/EnvironmentMap.h"
#include "../Backend/StaticModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include "gltext.h"


constexpr float changeX = 0.59f;
constexpr float changeY = 1.07f;
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
    PVE_MODE
};

static GameState currentState = MENU;
    
struct Game {

    Game();
    void Update();

    void printTuple();
    void printBoard(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
        
private:

    GLTtext* title = gltCreateText();
    GLTtext* Owins = gltCreateText();
    GLTtext* Xwins = gltCreateText();
    GLTtext* Draw = gltCreateText();

    std::vector<std::tuple<char,float,float>> check;
    std::array<std::pair<float, float>, 9> mapCoord = {
        std::make_pair(-0.87f, 0.55f), std::make_pair(-0.28f, 0.55f), std::make_pair(0.31f, 0.55f),
        std::make_pair(-0.87f, -0.52f), std::make_pair(-0.28f, -0.52f), std::make_pair(0.31f, -0.52f),
        std::make_pair(-0.87f, -1.59f), std::make_pair(-0.28f, -1.59f), std::make_pair(0.31f, -1.59f)
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
    Background background;
    GameMap map;
    SelectBorder border;
    ChooseMode mode;
    modeBorder modeBorder; 
    bool isPlayerTurn;

    // MENU INPUT //     
    void handleBorderInput();
    void goBack();
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
    void ResetGame(bool reset);
    bool PositionTaken(float x, float y);
    void updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y);
    unsigned int countPlayers(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> createEmptyBoard();
    void ClearBoard();
    char checkifPwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    char checkifEwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board);
    // BOARD LOGIC //
};
