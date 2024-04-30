#include <vector>
#include "Player.h"

class InstantiatePlayer {
public:
    InstantiatePlayer() {
        // Initialize the renderer
    }

    // Add a new image to render
    void AddPlayer(const Player& player) {
        players.push_back(player);
    }

    // Render all the added images
    void RenderAllPlayers() {
        for (auto& player : players) {
            player.Render();
        }
    }

private:
    std::vector<Player> players;
};
