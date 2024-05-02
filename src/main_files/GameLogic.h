#include <iostream>
#include <vector>
#include <utility>
#include <tuple>

#include "Shader.h"
#include "Input.h"
#include "GameMap.h"
#include "Title.h"
#include "Select.h"
#include "Player.h"
#include "Enemy.h"

namespace Logic {

    
    constexpr float changeX = 0.65f;
    constexpr float changeY = 1.17f;

    struct Game {

        Game() : isPlayerTurn(true) {}

        void initialize() {
            map.BindAndLoad();
            title.BindAndLoad();
            border.BindAndLoad();
            std::cout << "Your turn" << '\n';
        }

        void update() {
            title.Render();
            title.Delete();

            if (!title.render) {
                map.Render();
                border.Render();

                handleInput();
                

                InstPlay.RenderAllPlayers();
                InstEnem.RenderAllEnemys();
    
            }
        }

        void printMapCoord() {
            std::cout << "-----------" << '\n';
            for (const auto& i : mapCoord) {
                std::cout << i.first << " " << i.second << '\n';
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

    private:

        std::vector<std::pair<float, float>> mapCoord;
        std::vector<std::tuple<char,float,float>> check;

        GameMap map;
        StartScreen title;
        SelectBorder border;
        InstantiatePlayer InstPlay; 
        InstantiateEnemy InstEnem;  
        bool isPlayerTurn;

            void handleInput() {
                if (isPlayerTurn) {
                    handlePlayerInput(InstPlay);
                } else {
                    handleEnemyInput(InstEnem);
                }
            }

            void handlePlayerInput(InstantiatePlayer& instPlayer) {
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    border.UpdatePosition(0.0f, changeY);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    border.UpdatePosition(0.0f, -changeY);
                } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                    border.UpdatePosition(-changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                    border.UpdatePosition(changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_X)) {
                    float newX = border.getNewX();
                    float newY = border.getNewY();

                    if (!PositionTaken(newX, newY)) {
                        mapCoord.emplace_back(newX, newY);
                        check.emplace_back('P',newX, newY);
                        Player player; 
                        player.BindAndLoad();
                        player.UpdatePositionFromBorder(newX, newY);
                        instPlayer.AddPlayer(player);

                        if (isPlayerTurn) {
                            std::cout << "Player2 turn" << '\n';
                        } else {
                            std::cout << "Your turn" << '\n';
                        }

                        isPlayerTurn = !isPlayerTurn;
                    } else {
                        std::cout << "Position is taken" << '\n';
                    }
                }
            }

            void handleEnemyInput(InstantiateEnemy& instPlayer) {
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    border.UpdatePosition(0.0f, changeY);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    border.UpdatePosition(0.0f, -changeY);
                } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                    border.UpdatePosition(-changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                    border.UpdatePosition(changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_X)) {
                    float newX = border.getNewX();
                    float newY = border.getNewY();

                    if (!PositionTaken(newX, newY)) {
                        mapCoord.emplace_back(newX, newY);
                        check.emplace_back('E',newX, newY);
                        Enemy enemy;
                        enemy.BindAndLoad();
                        enemy.UpdatePositionFromBorder(newX, newY);
                        InstEnem.AddEnemy(enemy);

                        if (isPlayerTurn) {
                            std::cout << "Player2 turn" << '\n';
                        } else {
                            std::cout << "Your turn" << '\n';
                        }

                        isPlayerTurn = !isPlayerTurn;
                    } else {
                        std::cout << "Position is taken" << '\n';
                    }
                }
            }

            bool PositionTaken(float x, float y) {
                for (const auto& i : mapCoord) {
                    if (i.first == x && i.second == y) {
                        return true;
                    }
                }
                return false;
            }
    };
} 
