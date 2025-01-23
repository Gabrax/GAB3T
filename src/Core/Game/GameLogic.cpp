#include "GameLogic.h"
#include "glad/glad.h"

Game::Game() : isPlayerTurn(true) {
        Util::BakeShaders();
        board = createEmptyBoard();
        background.BindAndLoad();
        map.BindAndLoad();
        border.BindAndLoad();
        mode.BindAndLoad();
        modeBorder.BindAndLoad();
        gltInit();
        gltSetText(title, "Tic Tac Toe");
        gltSetText(Owins, "PLAYER 0 WINS");
        gltSetText(Xwins, "PLAYER X WINS");
        gltSetText(Draw, "DRAW");
        envMap.Bake();
        manager.AddLight(LightType::POINT, glm::vec4(1.0f), glm::vec3(5.0f,0.0f,5.0f));
    }

void Game::Update()
{
        
  switch (currentState) {

    case MENU:
      background.Render();
      // MENU //
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(title, (Window::GetWindowWidth()/2.0f) - 250.0f, 100, 5);
      gltEndDraw();

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
      glEnable(GL_DEPTH_TEST);
      manager.RenderLights();
      envMap.Render();
      boardModel->Render(glm::vec3(0.0f));
      border.Render();
      if (!isEnd) {
        PVPhandlePlayersInput();
      }

      for (const auto& circle : circles) {
        circle.Render();
      }
      for (const auto& cross : crosses) {
        cross.Render();
      }

      score = countPlayers(board);

      Pwins = checkifPwins(board);
      if (Pwins == 'P') {
      isEnd = true;
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Owins, (Window::GetWindowWidth()/2.0f) - 250.0f, 100, 5);
      gltEndDraw();
      }

      Ewins = checkifEwins(board);
      if (Ewins == 'E') {
      isEnd = true;
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Xwins, (Window::GetWindowWidth()/2.0f) - 250.0f, 100, 5);
      gltEndDraw();
      }

      if (Pwins != 'P' && Ewins != 'E' && score == 9) {
      isEnd = true;
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Draw, (Window::GetWindowWidth()/2.0f) - 250.0f, 100, 5);
      gltEndDraw();
      }

      if (Input::KeyPressed(GAB_KEY_R)) {
      ResetGame(isEnd);
      } else if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL)) {
      goBack();
      currentState = MENU;
      }
      // PVP PVP PVP //
  break;

    case PVE_MODE:
      // PVE PVE PVE //
      glEnable(GL_DEPTH_TEST);
      manager.RenderLights();
      envMap.Render();
      boardModel->Render(glm::vec3(0.0f));
      border.Render();
      if (!isEnd) {
      PVEhandlePlayersInput();
      }

      for (const auto& circle : circles) {
        circle.Render();
      }
      for (const auto& cross : crosses) {
        cross.Render();
      }

      score = countPlayers(board);

      Pwins = checkifPwins(board);
      if (Pwins == 'P') {
      isEnd = true;
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Owins, (Window::GetWindowWidth()/2.0f) - 250.0f, 50, 5);
      gltEndDraw();
      }

      Ewins = checkifEwins(board);
      if (Ewins == 'E') {
      isEnd = true;
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Xwins, (Window::GetWindowWidth()/2.0f) - 250.0f, 50, 5);
      gltEndDraw();
      }

      if (Pwins != 'P' && Ewins != 'E' && score == 9) {
      isEnd = true;
      gltBeginDraw();
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(Draw, (Window::GetWindowWidth()/2.0f) - 100.0f, 50, 5);
      gltEndDraw();
      }

      if (Input::KeyPressed(GAB_KEY_R)) {
      ResetGame(isEnd);
      } else if (Input::KeyPressed(GAB_KEY_RIGHT_CONTROL)) {
      goBack();
      currentState = MENU;
      }
      // PVE PVE PVE //
      break;
      }

}

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

void Game::handleBorderInput(){
  if (Input::KeyPressed(GAB_KEY_UP)) {
      modeBorder.UpdatePosition(0.0f, 1.0f);
  } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
      modeBorder.UpdatePosition(0.0f, -1.0f);
  }else if (Input::KeyPressed(GAB_KEY_ENTER)){
      if(std::abs(modeBorder.getNewY() + 0.1f) >= 1.0f){
          currentState = PVP_MODE;
          PvPmode = true;
          map.render = true;
          mode.render = false;
          modeBorder.render = false;
      }else if(std::abs(modeBorder.getNewY() + 0.1f) < 0.001f){
          currentState = PVE_MODE;
          PvEmode = true;
          map.render = true;
          mode.render = false;
          modeBorder.render = false;
      }
  }
}

void Game::goBack(){
  glDisable(GL_DEPTH_TEST);
  mode.render = true;
  modeBorder.render = true;

  PvPmode = false;
  PvEmode = false;
  map.render = false;
  isPlayerTurn = true;
  isEnd = false;
  
  check.clear();
  crosses.clear();
  circles.clear();
  ClearBoard();
  border.ResetPosition();
}
// MENU INPUT //

void Game::HandlePlayerMoving(){
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
void Game::PVPhandlePlayersInput() {
  if (isPlayerTurn) {
      handlePlayerInput();
  } else {
      handleEnemyInput();
  }
}

void Game::handlePlayerInput() {
  
  HandlePlayerMoving();

  if(Input::KeyPressed(GAB_KEY_ENTER)) {
      float newX = border.getNewX();
      float newY = border.getNewY();

      if (!PositionTaken(newX, newY)) {
          check.emplace_back('P',newX, newY);
          updateBoard(board, 'P', newX, newY);
          crosses.emplace_back(circleModel, glm::vec3(newX, newY, 0.0f));

          isPlayerTurn = !isPlayerTurn;
      }
  }
}

void Game::handleEnemyInput() {

  HandlePlayerMoving();

  if(Input::KeyPressed(GAB_KEY_ENTER)) {
      float newX = border.getNewX();
      float newY = border.getNewY();

      if (!PositionTaken(newX, newY)) {
          check.emplace_back('E',newX, newY);
          updateBoard(board, 'E', newX, newY);
          crosses.emplace_back(crossModel, glm::vec3(newX, newY, 0.0f));

          isPlayerTurn = !isPlayerTurn;
      }
  }
}
// PVP MODE //

// PVE MODE //
void Game::PVEhandlePlayersInput() {
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

          updateBoard(board, 'E', newX, newY);

          std::cout << "Evaluating move: (" << newX << ", " << newY << ")\n";

          int score = minimax(board, 9, false);

          std::cout << "Score for move (" << newX << ", " << newY << "): " << score << "\n";

          updateBoard(board, ' ', newX, newY);

          if (score > bestScore) {
              bestScore = score;
              bestX = newX;
              bestY = newY;
          }
      }

      // Print the best move chosen
      std::cout << "Best move chosen: (" << bestX << ", " << bestY << ") with score " << bestScore << "\n";

      // Make the best move
      check.emplace_back('E', bestX, bestY);
      updateBoard(board, 'E', bestX, bestY);
      crosses.emplace_back(crossModel, glm::vec3(bestX, bestY, 0.0f));

      isPlayerTurn = !isPlayerTurn;
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
void Game::ResetGame(bool reset){
  if(reset){

      isPlayerTurn = true;
      isEnd = false;
      board = createEmptyBoard();
      check.clear();
      crosses.clear();
      circles.clear();
  }
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

void Game::updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y) {
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


