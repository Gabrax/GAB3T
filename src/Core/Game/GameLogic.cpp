#include "GameLogic.h"
#include "glad/glad.h"
#include "../Backend/Multiplayer.h"
#include <limits>

Game::Game() : isPlayerTurn(true) {
        Util::BakeShaders();
        gltInit();
        gltSetText(title, "Tic Tac Toe");
        gltSetText(Owins, "PLAYER 0 WINS");
        gltSetText(Xwins, "PLAYER X WINS");
        gltSetText(Draw, "DRAW");
        gltSetText(PvsAI, "PvsAI");
        gltSetText(Local, "Local");
        gltSetText(Multiplayer, "Multiplayer");
        gltSetText(Host, "Host");
        gltSetText(Client, "Client");
        gltSetText(Connected, "Connected");
        gltSetText(Disconnected, "Disconnected");
        gltSetText(Host_Adress, "Connect to:");
        gltSetText(Waiting, "Waiting for client...");
        board = createEmptyBoard();
        envMap.Bake();
        manager.AddLight(LightType::POINT, glm::vec4(1.0f), lightPosition);
    }

void Game::Update()
{
        
  switch (currentState) {

    case MENU:
      // MENU //
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      static int selectedIndex = 0;
      if (Input::KeyPressed(GAB_KEY_UP) || Input::KeyPressed(GAB_KEY_W)) {
          selectedIndex = (selectedIndex - 1 + 3) % 3; 
      }
      if (Input::KeyPressed(GAB_KEY_DOWN) || Input::KeyPressed(GAB_KEY_S)) {
          selectedIndex = (selectedIndex + 1) % 3; 
      }
      if(Input::KeyPressed(GAB_KEY_ENTER) || Input::KeyPressed(GAB_KEY_SPACE)) {
        if (selectedIndex == 0) currentState = PVE_MODE;
        else if (selectedIndex == 1) currentState = PVP_MODE;
        else if (selectedIndex == 2) currentState = MULTI_MODE;

      }

      gltBeginDraw();
      // Title
      time = glfwGetTime(); 
      red = (sin(time * 0.5f) + 1.0f) * 0.5f;  
      green = (sin(time * 0.7f) + 1.0f) * 0.5f;
      blue = (sin(time * 1.0f) + 1.0f) * 0.5f;

      gltColor(red, green, blue, 1.0f);
      gltDrawText2D(title, (Window::GetWindowWidth()/2.0f) - 220.0f, 100, 5);

      if (selectedIndex == 0) gltColor(1.0f, 0.0f, 0.0f, 1.0f); 
      else gltColor(1.0f, 1.0f, 1.0f, 1.0f); 
      gltDrawText2D(PvsAI, (Window::GetWindowWidth()/2.0f) - 80.0f, 250, 4);

      if (selectedIndex == 1) gltColor(0.0f, 1.0f, 0.0f, 1.0f);
      else gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Local, (Window::GetWindowWidth()/2.0f) - 80.0f, 320, 4);

      if (selectedIndex == 2) gltColor(0.0f, 1.0f, 1.0f, 1.0f);
      else gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Multiplayer, (Window::GetWindowWidth()/2.0f) - 190.0f, 390, 4);

      gltEndDraw();

      glDisable(GL_BLEND);

    break;

    case PVP_MODE:
      // PVP PVP PVP //
      glEnable(GL_DEPTH_TEST);
        manager.RenderLights();
        time = glfwGetTime(); 
        x = orbitRadius * cos(orbitSpeed * time);
        y = orbitRadius * sin(orbitSpeed * time);
        lightPosition = glm::vec3(x, y, 2.0f);
        red = (sin(time * 0.5f) + 1.0f) * 0.5f;  
        green = (sin(time * 0.7f) + 1.0f) * 0.5f;
        blue = (sin(time * 1.0f) + 1.0f) * 0.5f;
        gradientColor = glm::vec4(red, green, blue, 1.0f);
        manager.EditLight(0,gradientColor,lightPosition);
        envMap.Render();
        boardModel->Render(glm::vec3(0.0f));

      if (!isEnd) {
        selectModel->Render(selectPos);
        PVPhandlePlayersInput();
      }

      for (const auto& circle : circles) circle.Render();
      for (const auto& cross : crosses) cross.Render();

      score = countPlayers(board);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      Pwins = checkifPwins(board);
      if (Pwins == 'P' && !isAnimating) {
        isEnd = true;
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Owins, (Window::GetWindowWidth()/2.0f) - 250.0f, 50, 5);
        gltEndDraw();
        for (auto& cross : crosses) cross.SetExplosion(true);
      }

      Ewins = checkifEwins(board);
      if (Ewins == 'E' && !isAnimating) {
        isEnd = true;
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Xwins, (Window::GetWindowWidth()/2.0f) - 250.0f, 50, 5);
        gltEndDraw();
        for (auto& circle : circles) circle.SetExplosion(true);
      }

      if (Pwins != 'P' && Ewins != 'E' && score == 9 && !isAnimating) {
        isEnd = true;
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Draw, (Window::GetWindowWidth()/2.0f) - 100.0f, 50, 5);
        gltEndDraw();
        for (auto& cross : crosses) cross.SetExplosion(true);
        for (auto& circle : circles) circle.SetExplosion(true);
      }
      glDisable(GL_BLEND);

      if (Input::KeyPressed(GAB_KEY_R) && isEnd) {
        ResetGame();
      } else if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL) || Input::KeyPressed(GAB_KEY_LEFT_CONTROL)) {
        ReturnToMenu();
      }
      // PVP PVP PVP //
    break;

    case PVE_MODE:
      // PVE PVE PVE //
      glEnable(GL_DEPTH_TEST);
        manager.RenderLights();
        time = glfwGetTime(); 
        x = orbitRadius * cos(orbitSpeed * time);
        y = orbitRadius * sin(orbitSpeed * time);
        lightPosition = glm::vec3(x, y, 2.0f);
        red = (sin(time * 0.5f) + 1.0f) * 0.5f;  // Oscillates between 0 and 1
        green = (sin(time * 0.7f) + 1.0f) * 0.5f;
        blue = (sin(time * 1.0f) + 1.0f) * 0.5f;
        gradientColor = glm::vec4(red, green, blue, 1.0f);
        manager.EditLight(0,gradientColor,lightPosition);
        envMap.Render();
        boardModel->Render(glm::vec3(0.0f));
      if (!isEnd) {
        selectModel->Render(selectPos);
        PVEhandlePlayersInput();
      }

      for (const auto& circle : circles) circle.Render();
      for (const auto& cross : crosses) cross.Render();

      score = countPlayers(board);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      Pwins = checkifPwins(board);
      if (Pwins == 'P' && !isAnimating) {
        isEnd = true;
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Owins, (Window::GetWindowWidth()/2.0f) - 250.0f, 50, 5);
        gltEndDraw();
        for (auto& cross : crosses) cross.SetExplosion(true);
      }

      Ewins = checkifEwins(board);
      if (Ewins == 'E' && !isAnimating) {
        isEnd = true;
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Xwins, (Window::GetWindowWidth()/2.0f) - 250.0f, 50, 5);
        gltEndDraw();
        for (auto& circle : circles) circle.SetExplosion(true);
      }

      if (Pwins != 'P' && Ewins != 'E' && score == 9 && !isAnimating) {
        isEnd = true;
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Draw, (Window::GetWindowWidth()/2.0f) - 100.0f, 50, 5);
        gltEndDraw();
        for (auto& cross : crosses) cross.SetExplosion(true);
        for (auto& circle : circles) circle.SetExplosion(true);
      }
      glDisable(GL_BLEND);

      if (Input::KeyPressed(GAB_KEY_R) && isEnd) {
        ResetGame();
      } else if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL) || Input::KeyPressed(GAB_KEY_LEFT_CONTROL)) {
        ReturnToMenu();
      }
      // PVE PVE PVE //
    break;

    
    case MULTI_MODE:

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static int selectedIndex1 = 0;

    if (!MP::_Host && !MP::_Client) {
        if (Input::KeyPressed(GAB_KEY_UP) || Input::KeyPressed(GAB_KEY_W)) {
            selectedIndex1 = (selectedIndex1 - 1 + 2) % 2; 
        }
        if (Input::KeyPressed(GAB_KEY_DOWN) || Input::KeyPressed(GAB_KEY_S)) {
            selectedIndex1 = (selectedIndex1 + 1) % 2; 
        }
        if (Input::KeyPressed(GAB_KEY_ENTER) || Input::KeyPressed(GAB_KEY_SPACE)) {
            if (selectedIndex1 == 0) MP::_Host = true;
            else if (selectedIndex1 == 1) MP::_Client = true;
            MP::hasTriedConnecting = false;
            MP::isConnected = false;
            MP::inputDebounce = true; // Activate debounce
            MP::debounceStartTime = glfwGetTime(); // Store the time of selection
        }

        gltBeginDraw();
        time = glfwGetTime();
        red = (sin(time * 0.5f) + 1.0f) * 0.5f;  
        green = (sin(time * 0.7f) + 1.0f) * 0.5f;
        blue = (sin(time * 1.0f) + 1.0f) * 0.5f;

        gltColor(red, green, blue, 1.0f);
        gltDrawText2D(title, (Window::GetWindowWidth()/2.0f) - 220.0f, 100, 5);

        gltColor(selectedIndex1 == 0 ? 1.0f : 1.0f, selectedIndex1 == 0 ? 0.0f : 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Host, (Window::GetWindowWidth()/2.0f) - 80.0f, 250, 4);

        gltColor(selectedIndex1 == 1 ? 0.0f : 1.0f, selectedIndex1 == 1 ? 1.0f : 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Client, (Window::GetWindowWidth()/2.0f) - 80.0f, 320, 4);
        gltEndDraw();
    }

    if (MP::_Host && !MP::hasTriedConnecting) {
        MP::hasTriedConnecting = true;
        MP::isConnected = MP::Init_Host();
    } 
    else if (MP::_Client && !MP::hasTriedConnecting) {
        MP::hasTriedConnecting = true;
        MP::isConnected = MP::Join_Client();
    }

    if (MP::_Host) UpdateHost();
    if (MP::_Client) UpdateClient();

    if (MP::_Client && !MP::isConnected) {
        MP::Cleanup();
        MP::_Client = false;
        MP::hasTriedConnecting = false;
        ReturnToMenu();
        break;
    }

    if (MP::isConnected) { 
        if (!isEnd) {
            if (MP::_Client || (MP::_Host && MP::HasClientConnected())) { 
                selectModel->Render(selectPos);
                MULTIhandlePlayersInput();
            }
        }

        score = countPlayers(board);
        Pwins = checkifPwins(board);
        Ewins = checkifEwins(board);

        if (Pwins == 'P' && !isAnimating) {
            isEnd = true;
            for (auto& cross : crosses) cross.SetExplosion(true);
        }
        if (Ewins == 'E' && !isAnimating) {
            isEnd = true;
            for (auto& circle : circles) circle.SetExplosion(true);
        }
        if (Pwins != 'P' && Ewins != 'E' && score == 9 && !isAnimating) {
            isEnd = true;
            for (auto& cross : crosses) cross.SetExplosion(true);
            for (auto& circle : circles) circle.SetExplosion(true);
        }

        glEnable(GL_DEPTH_TEST);
        manager.RenderLights();

        time = glfwGetTime();
        x = orbitRadius * cos(orbitSpeed * time);
        y = orbitRadius * sin(orbitSpeed * time);
        lightPosition = glm::vec3(x, y, 2.0f);
        red = (sin(time * 0.5f) + 1.0f) * 0.5f;
        green = (sin(time * 0.7f) + 1.0f) * 0.5f;
        blue = (sin(time * 1.0f) + 1.0f) * 0.5f;
        gradientColor = glm::vec4(red, green, blue, 1.0f);
        manager.EditLight(0, gradientColor, lightPosition);
        envMap.Render();
        boardModel->Render(glm::vec3(0.0f));

        for (const auto& circle : circles) circle.Render();
        for (const auto& cross : crosses) cross.Render();

        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D((MP::_Host && !MP::HasClientConnected()) ? Waiting : 
                      (MP::isConnected ? Connected : Disconnected), 10.0f, Window::GetWindowHeight()-60.0f, 4);    
        gltEndDraw();
    }

    glDisable(GL_BLEND);

    if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL) || Input::KeyPressed(GAB_KEY_LEFT_CONTROL)) {
        MP::Cleanup();
        MP::_Host = false;
        MP::_Client = false;
        MP::hasTriedConnecting = false;
        MP::isConnected = false;
        ReturnToMenu();
    }

    if (Input::KeyPressed(GAB_KEY_R) && isEnd) {
        ResetGame();
    }

    break;  
  }

}

#ifdef DEBUG
void Game::printTuple() {
    std::cout << "-----------" << '\n';
    for (const auto& tuple : check) {
    
        char ch = std::get<0>(tuple);
        float val1 = std::get<1>(tuple);
        float val2 = std::get<2>(tuple);

        std::cout << ch << " " << val1 << " " << val2 << '\n';
    }
}

void Game::printBoard(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
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
#endif

void Game::ReturnToMenu(){
  glDisable(GL_DEPTH_TEST);
  currentState = MENU;
  isPlayerTurn = true;
  isEnd = false;
  
  check.clear();
  crosses.clear();
  circles.clear();
  ClearBoard();
  selectPos = glm::vec3(0.0f);
}
// MENU INPUT //

void Game::HandlePlayerMoving() {
    if (Input::KeyPressed(GAB_KEY_UP) || Input::KeyPressed(GAB_KEY_W)) {
        if (selectPos.y + changeY <= 2.1f) selectPos.y += changeY;
    } else if (Input::KeyPressed(GAB_KEY_DOWN) || Input::KeyPressed(GAB_KEY_S)) {
        if (selectPos.y - changeY >= -2.1f) selectPos.y -= changeY;
    } else if (Input::KeyPressed(GAB_KEY_LEFT) || Input::KeyPressed(GAB_KEY_A)) {
        if (selectPos.x - changeX >= -2.1f) selectPos.x -= changeX;
    } else if (Input::KeyPressed(GAB_KEY_RIGHT) || Input::KeyPressed(GAB_KEY_D)) {
        if (selectPos.x + changeX <= 2.1f) selectPos.x += changeX;
    }
}

// PVP MODE //
void Game::PVPhandlePlayersInput() {
    if (isAnimating) {
        handleAnimation();
        return; // Block input while animation is running
    }

    if (isPlayerTurn) {
        handlePlayerInput();
    } else {
        handleEnemyInput();
    }
}

void Game::MULTIhandlePlayersInput() {
    if (isAnimating) {
        handleAnimation();
        return; // Block input while animation is running
    }

    if (MP::_Host)  handlePlayerInput();
    if (MP::_Client){
      if (MP::inputDebounce && (glfwGetTime() - MP::debounceStartTime < 0.2)) return; // Ignore input for 200ms

      MP::inputDebounce = false; // Reset debounce after timeout
      handleEnemyInput();
    }  
}

void Game::handlePlayerInput() {
    HandlePlayerMoving();

    if (Input::KeyPressed(GAB_KEY_ENTER) || Input::KeyPressed(GAB_KEY_SPACE)) {
        if (!PositionTaken(selectPos.x, selectPos.y)) {
            check.emplace_back('P', selectPos.x, selectPos.y);
            updateBoard(board, 'P', selectPos.x, selectPos.y);
            circles.emplace_back(circleModel, glm::vec3(selectPos.x, selectPos.y, animationZ));
            animationZ = 2.5f;
            isAnimating = true;
            animationStart = std::chrono::steady_clock::now();
            if (MP::_Host) {
              SendMoveToClient(selectPos.x, selectPos.y, 'P');
              puts("SEND TO CLIENT"); 
            }
        }
    }
}

void Game::handleEnemyInput() {
    HandlePlayerMoving();

    if (Input::KeyPressed(GAB_KEY_ENTER) || Input::KeyPressed(GAB_KEY_SPACE)) {
        if (!PositionTaken(selectPos.x, selectPos.y)) {
            check.emplace_back('E', selectPos.x, selectPos.y);
            updateBoard(board, 'E', selectPos.x, selectPos.y);
            crosses.emplace_back(crossModel, glm::vec3(selectPos.x, selectPos.y, animationZ));
            animationZ = 2.5f;
            isAnimating = true;
            animationStart = std::chrono::steady_clock::now();
            if (MP::_Client) {
              SendMoveToHost(selectPos.x, selectPos.y, 'E'); 
              puts("SEND TO HOST");
            }
        } 
    }
}
// PVP MODE //
void Game::handleAnimation() {
    auto now = std::chrono::steady_clock::now();
    float elapsedTime = std::chrono::duration<float>(now - animationStart).count();

    // Calculate the current Z position based on elapsed time and animation speed
    animationZ -= animationSpeed * elapsedTime;

    // Ensure it doesn't overshoot Z = 0.0f
    if (animationZ <= 0.0f) {
        animationZ = 0.0f;
        isAnimating = false; // Animation is complete
        isPlayerTurn = !isPlayerTurn; // Switch turns
        return;
    }

    if (!circles.empty() && std::get<0>(check.back()) == 'P') {
        circles.back().GetPosition().z = animationZ;
    } else if (!crosses.empty() && std::get<0>(check.back()) == 'E') {
        crosses.back().GetPosition().z = animationZ;
    }
}
// PVE MODE //
void Game::PVEhandlePlayersInput() {

  if (isAnimating) {
      handleAnimation();
      return; 
  }

  if (isPlayerTurn) {
      handlePlayerInput();
  } else {
      handleAiInput();
  }
}

void Game::handleAiInput() {
  if (!isPlayerTurn) {
      float bestX = 0.0f;
      float bestY = 0.0f;
      int bestScore = std::numeric_limits<int>::min();

      // Generate all possible moves for the AI player
      std::vector<std::tuple<float, float, float>> possibleMoves;
      for (const auto& coord : mapCoord) {
          float newX = std::get<0>(coord);
          float newY = std::get<1>(coord);
          if (!PositionTaken(newX, newY)) {
              possibleMoves.emplace_back(newX, newY, 0.0f);
          }
      }

      for (const auto& move : possibleMoves) {
          float newX = std::get<0>(move);
          float newY = std::get<1>(move);

          updateBoard(board, 'E', newX, newY);
#ifdef DEBUG
          std::cout << "Evaluating move: (" << newX << ", " << newY << ")\n";
#endif 

          int score = minimax(board, 9, false);
#ifdef DEBUG
          std::cout << "Score for move (" << newX << ", " << newY << "): " << score << "\n";
#endif 

          updateBoard(board, ' ', newX, newY);

          if (score > bestScore) {
              bestScore = score;
              bestX = newX;
              bestY = newY;
          }
      }

#ifdef DEBUG
      std::cout << "Best move chosen: (" << bestX << ", " << bestY << ") with score " << bestScore << "\n";
#endif
      // Make the best move
      check.emplace_back('E', bestX, bestY);
      updateBoard(board, 'E', bestX, bestY);
      crosses.emplace_back(crossModel, glm::vec3(bestX, bestY, animationZ));
      animationZ = 2.5f;
      isAnimating = true;
      animationStart = std::chrono::steady_clock::now();
  }
}

int Game::minimax(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, int depth, bool maximizingPlayer) {
  int score = evaluate(board);
  if (score == 10 || score == -10) return score;
  if (depth == 0 || gameIsOver(board)) return 0;

  if (maximizingPlayer) {
      int maxScore = std::numeric_limits<int>::min();
      for (auto& move : generateMoves(board, 'E')) {
          maxScore = std::max(maxScore, minimax(move, depth - 1, false) - depth);
      }
      return maxScore;
  } else {
      int minScore = std::numeric_limits<int>::max();
      for (auto& move : generateMoves(board, 'P')) {
          minScore = std::min(minScore, minimax(move, depth - 1, true) + depth);
      }
      return minScore;
  }
}

std::vector<std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>> Game::generateMoves(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player) {
  std::vector<std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>> moves;
  for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
          if (board[i][j] == ' ') {
              std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> newBoard = board;
              newBoard[i][j] = player;
              moves.push_back(newBoard);
          }
      }
  }
  return moves;
}

int Game::evaluate(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
  // Check rows and columns for a win
  for (int i = 0; i < BOARD_SIZE; ++i) {
      if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
          if (board[i][0] == 'E') return 10;
          else if (board[i][0] == 'P') return -10;
      }
      if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
          if (board[0][i] == 'E') return 10;
          else if (board[0][i] == 'P') return -10;
      }
  }
  // Check diagonals for a win
  if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
      if (board[0][0] == 'E') return 10;
      else if (board[0][0] == 'P') return -10;
  }
  if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
      if (board[0][2] == 'E') return 10;
      else if (board[0][2] == 'P') return -10;
  }
  // If no winner, return 0
  return 0;
}

bool Game::gameIsOver(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
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
void Game::ResetGame(){
    isAnimating = false;
    isPlayerTurn = true;
    isEnd = false;
    board = createEmptyBoard();
    check.clear();
    crosses.clear();
    circles.clear();
    selectPos = glm::vec3(0.0f);
}

bool Game::PositionTaken(float x, float y) {
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

void Game::SendMoveToClient(int row, int col, char player) {
    if (!MP::_Host || !MP::clientConnected) return;

    std::string moveMessage = "MOVE|" + std::to_string(player) + "|" + std::to_string(row) + "|" + std::to_string(col);
    ENetPacket* packet = enet_packet_create(moveMessage.c_str(), moveMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(MP::serverPeer, 0, packet);
    enet_host_flush(MP::server);
}

void Game::SendMoveToHost(int row, int col, char player) {
    if (!MP::_Client || !MP::isConnected) return;

    std::string moveMessage = "MOVE|" + std::to_string(player) + "|" + std::to_string(row) + "|" + std::to_string(col);
    ENetPacket* packet = enet_packet_create(moveMessage.c_str(), moveMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(MP::clientPeer, 0, packet);
    enet_host_flush(MP::client);
}

void Game::updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y) {
  // Find the closest coordinates in mapCoord
  float minDist = std::numeric_limits<float>::max();
  int closestIndex = -1;
  for (size_t i = 0; i < mapCoord.size(); ++i) {
      float dist = std::hypot(x - mapCoord[i].first, y - mapCoord[i].second); 
      if (dist < minDist) {
          minDist = dist;
          closestIndex = static_cast<int>(i);
      }
  }

  if (closestIndex != -1) {
      int row = closestIndex / BOARD_SIZE;
      int col = closestIndex % BOARD_SIZE;
      board[row][col] = player;
  }
}

void Game::UpdateHost()
{
    if (!MP::server) return;

    ENetEvent event;
    while (enet_host_service(MP::server, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                MP::serverPeer = event.peer;
                MP::clientConnected = true;
                std::cout << "Client connected from " 
                          << event.peer->address.host << ":" 
                          << event.peer->address.port << "\n";
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected.\n";
                MP::clientConnected = false;
                MP::serverPeer = nullptr;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
            {
                std::string data(reinterpret_cast<char*>(event.packet->data));
                enet_packet_destroy(event.packet);

                if (data.rfind("MOVE", 0) == 0) { 
                    int playerID, row, col;
                    sscanf_s(data.c_str(), "MOVE|%d|%d|%d", &playerID, &row, &col);
                 
                    if (playerID == 69 && !PositionTaken(row, col)) {
                        check.emplace_back('E', row, col);
                        updateBoard(board, 'E', row, col);

                        crosses.emplace_back(crossModel, glm::vec3(row, col, animationZ));
                        animationZ = 2.5f;
                        isAnimating = true;
                        animationStart = std::chrono::steady_clock::now();
                    }
                }
                break;
            }

            default:
                break;
        }
    }
}

void Game::UpdateClient()
{
    if (!MP::client) return;

    ENetEvent event;
    while (enet_host_service(MP::client, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server.\n";
                MP::isConnected = false;
                MP::clientPeer = nullptr;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
            {
                std::string data(reinterpret_cast<char*>(event.packet->data));
                enet_packet_destroy(event.packet);

                if (data.rfind("MOVE", 0) == 0) {
                    int playerID, row, col;
                    sscanf_s(data.c_str(), "MOVE|%d|%d|%d", &playerID, &row, &col);
                    if (playerID == 80 && !PositionTaken(row, col)) {
                        check.emplace_back('P', row, col);
                        updateBoard(board, 'P', row, col);

                        circles.emplace_back(circleModel, glm::vec3(row, col, animationZ));
                        animationZ = 2.5f;
                        isAnimating = true;
                        animationStart = std::chrono::steady_clock::now();

                    }
                }
                break;
            }

            default:
                break;
        }
    }
}

unsigned int Game::countPlayers(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board){
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

std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> Game::createEmptyBoard() {
  std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> emptyBoard;

  for (auto& row : emptyBoard) {
      for (char& cell : row) {
          cell = ' ';
      }
  }
  return emptyBoard;
}

void Game::ClearBoard(){
  for(auto& i : board){
      i.fill(' ');
  }
}

char Game::checkifPwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
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

char Game::checkifEwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
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


