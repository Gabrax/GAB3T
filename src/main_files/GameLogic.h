#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <cmath>

#include "Shader.h"
#include "Input.h"
#include "GameMap.h"
#include "Title.h"
#include "Select.h"
#include "Player.h"
#include "Enemy.h"
#include "WinBox.h"
#include "ChooseMode.h"
#include "modeBorder.h"
#include "Background.h"

namespace Logic {

    constexpr float changeX = 0.59f;
    constexpr float changeY = 1.07f;
    constexpr int BOARD_SIZE = 3;
    bool isEnd = false;
    bool PvPmode = false;
    bool PvEmode = false;
    static char Pwins;
    static char Ewins;
    static int score;

    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> board;

    enum GameState {
        TITLE,
        MODE_SELECTION,
        PVP_MODE,
        PVE_MODE
    };

    GameState currentState = TITLE;
    
    struct Game {

        Game() : isPlayerTurn(true) {
            board = createEmptyBoard();
        }

        void initialize() {
            background.BindAndLoad();
            map.BindAndLoad();
            title.BindAndLoad();
            border.BindAndLoad();
            mode.BindAndLoad();
            modeBorder.BindAndLoad();
            box.BindAndLoad();
        }

        void update() {

            background.Render();

            switch (currentState) {
                case TITLE:
                    title.Render();
                    if (Input::KeyPressed(GAB_KEY_ENTER)) { 
                        title.Delete();
                        currentState = MODE_SELECTION;
                    }
                    break;

                case MODE_SELECTION:
                    // MENU //
                    mode.render = true;
                    mode.Render();
                    modeBorder.render = true;
                    modeBorder.Render();

                    if (modeBorder.render && mode.render) {
                        handleBorderInput();
                    }
                    break;

                case PVP_MODE:
                    // PVP PVP PVP //
                    map.Render();
                    border.Render();
                    if (!isEnd) {
                        PVPhandlePlayersInput();
                    }

                    InstPlay.RenderAllPlayers();
                    InstEnem.RenderAllenemies();

                    score = countPlayers(board);

                    Pwins = checkifPwins(board);
                    if (Pwins == 'P') {
                        isEnd = true;
                        box.Pwin = true;
                        box.Render();
                    }

                    Ewins = checkifEwins(board);
                    if (Ewins == 'E') {
                        isEnd = true;
                        box.Ewin = true;
                        box.Render();
                    }

                    if (Pwins != 'P' && Ewins != 'E' && score == 9) {
                        isEnd = true;
                        box.Draw = true;
                        box.Render();
                    }

                    if (Input::KeyPressed(GAB_KEY_R)) {
                        ResetGame(isEnd);
                    } else if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL)) {
                        goBack();
                        currentState = MODE_SELECTION;
                    }
                    // PVP PVP PVP //
                    break;

                case PVE_MODE:
                    // PVE PVE PVE //
                    map.Render();
                    border.Render();
                    if (!isEnd) {
                        PVEhandlePlayersInput();
                    }

                    InstPlay.RenderAllPlayers();
                    InstEnem.RenderAllenemies();

                    score = countPlayers(board);

                    Pwins = checkifPwins(board);
                    if (Pwins == 'P') {
                        isEnd = true;
                        box.Pwin = true;
                        box.Render();
                    }

                    Ewins = checkifEwins(board);
                    if (Ewins == 'E') {
                        isEnd = true;
                        box.Ewin = true;
                        box.Render();
                    }

                    if (Pwins != 'P' && Ewins != 'E' && score == 9) {
                        isEnd = true;
                        box.Draw = true;
                        box.Render();
                    }

                    if (Input::KeyPressed(GAB_KEY_R)) {
                        ResetGame(isEnd);
                    } else if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL)) {
                        goBack();
                        currentState = MODE_SELECTION;
                    }
                    // PVE PVE PVE //
                    break;
            }
        }

        void printTuple() {
            std::cout << "-----------" << '\n';
            for (const auto& tuple : check) {
            
                char ch = std::get<0>(tuple);
                float val1 = std::get<1>(tuple);
                float val2 = std::get<2>(tuple);

                std::cout << ch << " " << val1 << " " << val2 << '\n';
            }
        }
        
            void printBoard(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        std::cout << board[i][j];
                        if (j < BOARD_SIZE - 1) {
                            std::cout << " | ";
                        }
                    }
                    std::cout << std::endl;
                    if (i < BOARD_SIZE - 1) {
                        std::cout << "---------" << std::endl;
                    }
                }
            }

            

    private:

        std::vector<std::tuple<char,float,float>> check;
        std::vector<std::pair<float,float>> mapCoord = {
            {-0.87f, 0.55f},  {-0.28f, 0.55f},  {0.31f, 0.55f},
            {-0.87f, -0.52f}, {-0.28f, -0.52f}, {0.31f, -0.52f},
            {-0.87f, -1.59f}, {-0.28f, -1.59f}, {0.31f, -1.59f}
        };
        
        Background background;
        GameMap map;
        StartScreen title;
        SelectBorder border;
        InstantiatePlayer InstPlay; 
        InstantiateEnemy InstEnem; 
        ChooseMode mode;
        modeBorder modeBorder; 
        WinBox box;
        bool isPlayerTurn;

            // MENU INPUT //     
            void handleBorderInput(){
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    modeBorder.UpdatePosition(0.0f, 1.0f);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    modeBorder.UpdatePosition(0.0f, -1.0f);
                }else if (Input::KeyPressed(GAB_KEY_ENTER)){
                    if(std::abs(modeBorder.getNewY() + 0.1f) >= 1.0f){
                        currentState = PVP_MODE;
                        PvPmode = true;
                        box.render = true;
                        map.render = true;
                        mode.render = false;
                        modeBorder.render = false;
                    }else if(std::abs(modeBorder.getNewY() + 0.1f) < 0.001f){
                        currentState = PVE_MODE;
                        PvEmode = true;
                        box.render = true;
                        map.render = true;
                        mode.render = false;
                        modeBorder.render = false;
                    }
                }
            }

            void goBack(){
                mode.render = true;
                modeBorder.render = true;

                PvPmode = false;
                PvEmode = false;
                map.render = false;
                isPlayerTurn = true;
                isEnd = false;
                box.render = false;
                box.Pwin = false;
                box.Ewin = false;
                box.Draw = false;
                
                check.clear();
                InstPlay.clearPlayer();
                InstEnem.clearEnemy();
                ClearBoard();
                border.ResetPosition();
            }
            // MENU INPUT //

            void HandlePlayerMoving(){
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    border.UpdatePosition(0.0f, changeY);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    border.UpdatePosition(0.0f, -changeY);
                } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                    border.UpdatePosition(-changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                    border.UpdatePosition(changeX, 0.0f);
                }
            }

            // PVP MODE //
            void PVPhandlePlayersInput() {
                if (isPlayerTurn) {
                    handlePlayerInput(InstPlay);
                } else {
                    handleEnemyInput(InstEnem);
                }
            }

            void handlePlayerInput(InstantiatePlayer& instPlayer) {
                
                HandlePlayerMoving();

                if(Input::KeyPressed(GAB_KEY_ENTER)) {
                    float newX = border.getNewX();
                    float newY = border.getNewY();

                    if (!PositionTaken(newX, newY)) {
                        check.emplace_back('P',newX, newY);
                        updateBoard(board, 'P', newX, newY);
                        Player player; 
                        player.BindAndLoad();
                        player.UpdatePositionFromBorder(newX, newY);
                        instPlayer.AddPlayer(player);

                        isPlayerTurn = !isPlayerTurn;
                    }
                }
            }

            void handleEnemyInput(InstantiateEnemy& InstEnem) {

                HandlePlayerMoving();

                if(Input::KeyPressed(GAB_KEY_ENTER)) {
                    float newX = border.getNewX();
                    float newY = border.getNewY();

                    if (!PositionTaken(newX, newY)) {
                        check.emplace_back('E',newX, newY);
                        updateBoard(board, 'E', newX, newY);
                        Enemy enemy;
                        enemy.BindAndLoad();
                        enemy.UpdatePositionFromBorder(newX, newY);
                        InstEnem.AddEnemy(enemy);

                        isPlayerTurn = !isPlayerTurn;
                    }
                }
            }
            // PVP MODE //

            // PVE MODE //
            void PVEhandlePlayersInput() {
                if (isPlayerTurn) {
                    handlePlayerInput(InstPlay);
                } else {
                    handleAiInput(InstEnem);
                }
            }

            void handleAiInput(InstantiateEnemy& InstEnem) {
                if (!isPlayerTurn) {

                    const int depth = 3;
                    
                    float bestX = 0.0f;
                    float bestY = 0.0f;
                    int bestScore = std::numeric_limits<int>::min();

                    // Generate all possible moves for the AI player
                    std::vector<std::tuple<float, float>> possibleMoves;
                    for (const auto& coord : mapCoord) {
                        float newX = std::get<0>(coord);
                        float newY = std::get<1>(coord);
                        if (!PositionTaken(newX, newY)) {
                            possibleMoves.emplace_back(newX, newY);
                        }
                    }

                    
                    for (const auto& move : possibleMoves) {
                        float newX = std::get<0>(move);
                        float newY = std::get<1>(move);

                        // Emulate making the move
                        updateBoard(board, 'E', newX, newY);

                        
                        int score = minimax(board, depth, false); 

                        // Undo the move
                        updateBoard(board, ' ', newX, newY);

                        // Update the best move if this move has a higher score
                        if (score > bestScore) {
                            bestScore = score;
                            bestX = newX;
                            bestY = newY;
                        }
                    }

                    // Make the best move
                    check.emplace_back('E', bestX, bestY);
                    updateBoard(board, 'E', bestX, bestY);
                    Enemy enemy;
                    enemy.BindAndLoad();
                    enemy.UpdatePositionFromBorder(bestX, bestY);
                    InstEnem.AddEnemy(enemy);

                    isPlayerTurn = !isPlayerTurn; 
                }
            }

            int minimax(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, int depth, bool maximizingPlayer) {
                
                if (depth == 0 || gameIsOver(board)) {
                    return evaluate(board);
                }

                if (maximizingPlayer) {
                    int maxScore = std::numeric_limits<int>::min();
                    for (auto& move : generateMoves(board)) {
                        maxScore = std::max(maxScore, minimax(move, depth - 1, false));
                    }
                    return maxScore;
                } else {
                    int minScore = std::numeric_limits<int>::max();
                    for (auto& move : generateMoves(board)) {
                        minScore = std::min(minScore, minimax(move, depth - 1, true));
                    }
                    return minScore;
                }
            }

            
            std::vector<std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>> generateMoves(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                std::vector<std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>> moves;
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        if (board[i][j] == ' ') {
                            std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> newBoard = board;
                            newBoard[i][j] = 'E'; // Assuming player is X
                            moves.push_back(newBoard);
                        }
                    }
                }
                return moves;
            }

            // Function to evaluate game state
            int evaluate(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                        if (board[i][0] == 'E') return 10;
                        else if (board[i][0] == 'P') return -10;
                    }
                }
                
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
                        if (board[0][j] == 'E') return 10;
                        else if (board[0][j] == 'P') return -10;
                    }
                }
                
                if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
                    if (board[0][0] == 'E') return 10;
                    else if (board[0][0] == 'P') return -10;
                }
                if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
                    if (board[0][2] == 'E') return 10;
                    else if (board[0][2] == 'P') return -10;
                }
                
                return 0;
            }

            bool gameIsOver(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                // Check if any player has won
                int eval = evaluate(board);
                if (eval == 10 || eval == -10) return true;

                // Check if the board is full (draw)
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        if (board[i][j] == ' ') return false;
                    }
                }
                return true;
            }

            // PVE MODE // 

            // BOARD LOGIC //
            void ResetGame(bool reset){
                if(reset = true){
                    isPlayerTurn = true;
                    isEnd = false;
                    board = createEmptyBoard();
                    check.clear();
                    InstEnem.Delete();
                    InstPlay.Delete();
                }
            }

            bool PositionTaken(float x, float y) {
                for (const auto& i : check) {
                    float val1 = std::get<1>(i);
                    float val2 = std::get<2>(i);
                    //std::cout << "Comparing (" << x << ", " << y << ") with (" << val1 << ", " << val2 << ")" << std::endl;
                    if (std::abs(val1 - x) < 0.01f && std::abs(val2 - y) < 0.01f) {
                        return true;
                    }
                }
                return false;
            }

            void updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y) {
                // Find the closest coordinates in mapCoord
                float minDist = std::numeric_limits<float>::max();
                int closestIndex = -1;
                for (size_t i = 0; i < mapCoord.size(); ++i) {
                    float dist = std::hypot(x - mapCoord[i].first, y - mapCoord[i].second); //Computes the square root of the sum of the squares of x and y
                    if (dist < minDist) {
                        minDist = dist;
                        closestIndex = static_cast<int>(i);
                    }
                }

                // Map the closest coordinates to the corresponding cell on the board
                if (closestIndex != -1) {
                    int row = closestIndex / BOARD_SIZE;
                    int col = closestIndex % BOARD_SIZE;
                    board[row][col] = player;
                }
            }

            unsigned int countPlayers(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board){
                unsigned int count = 0;
                for(const auto& i : board){
                    for(char j : i){
                        if(j == 'P' || j == 'E'){
                            count++;
                        }
                    }
                }
                return count;
            }

            std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> createEmptyBoard() {
                std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> emptyBoard;

                for (auto& row : emptyBoard) {
                    for (char& cell : row) {
                        cell = ' ';
                    }
                }
                return emptyBoard;
            }

            void ClearBoard(){
                for(auto& i : board){
                    i.fill(' ');
                }
            }

            char checkifPwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                // Check horizontally
                for (const auto& row : board) {
                    int count = 0;
                    for (char cell : row) {
                        if (cell == 'P') {
                            count++;
                            if (count == 3) return 'P'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check vertically
                for (size_t col = 0; col < BOARD_SIZE; col++) {
                    int count = 0;
                    for (size_t row = 0; row < BOARD_SIZE; row++) {
                        if (board[row][col] == 'P') {
                            count++;
                            if (count == 3) return 'P'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check diagonally (top-left to bottom-right)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = 0; j <= BOARD_SIZE - 3; j++) {
                        if (board[i][j] == 'P' && board[i+1][j+1] == 'P' && board[i+2][j+2] == 'P') {
                            return 'P'; 
                        }
                    }
                }

                // Check diagonally (top-right to bottom-left)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = BOARD_SIZE - 1; j >= 2; j--) {
                        if (board[i][j] == 'P' && board[i+1][j-1] == 'P' && board[i+2][j-2] == 'P') {
                            return 'P'; 
                        }
                    }
                }

                // If no winner found
                return '\0';
            }

            char checkifEwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                // Check horizontally
                for (const auto& row : board) {
                    int count = 0;
                    for (char cell : row) {
                        if (cell == 'E') {
                            count++;
                            if (count == 3) return 'E'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check vertically
                for (size_t col = 0; col < BOARD_SIZE; col++) {
                    int count = 0;
                    for (size_t row = 0; row < BOARD_SIZE; row++) {
                        if (board[row][col] == 'E') {
                            count++;
                            if (count == 3) return 'E'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check diagonally (top-left to bottom-right)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = 0; j <= BOARD_SIZE - 3; j++) {
                        if (board[i][j] == 'E' && board[i+1][j+1] == 'E' && board[i+2][j+2] == 'E') {
                            return 'E'; 
                        }
                    }
                }

                // Check diagonally (top-right to bottom-left)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = BOARD_SIZE - 1; j >= 2; j--) {
                        if (board[i][j] == 'E' && board[i+1][j-1] == 'E' && board[i+2][j-2] == 'E') {
                            return 'E'; 
                        }
                    }
                }

                // If no winner found
                return '\0';
            }
            // BOARD LOGIC //
    };
} 
