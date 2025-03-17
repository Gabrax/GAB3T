#pragma once
#define NOMINMAX
#include <enet/enet.h>
#include <iostream>

namespace MP {

    inline bool _Host = false;
    inline bool _Client = false;
    inline bool hasTriedConnecting = false; 
    inline bool isConnected = false; 
    inline ENetHost* server = nullptr;
    inline ENetHost* client = nullptr;
    inline ENetPeer* serverPeer = nullptr;
    inline ENetPeer* clientPeer = nullptr;
    inline bool clientConnected = false;
    inline bool inputDebounce = false; 
    inline double debounceStartTime = 0.0;
    inline bool HostTurn = true;

    inline bool HasClientConnected() {
        return clientConnected;
    }

    inline void OnClientJoin() { 
        clientConnected = true;
        std::cout << "Client has joined the game.\n";
    }

    inline void OnClientLeave() { 
        clientConnected = false;
        std::cout << "Client has disconnected.\n";
    }

    inline bool Init_Host()
    {
        if (enet_initialize() != 0)
        {
            std::cerr << "An error occurred while initializing ENet.\n";
            return false;
        }
        puts("Initialized ENet");

        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = 1234;

        server = enet_host_create(&address, 32, 2, 0, 0);
        if (server == nullptr)
        {
            std::cerr << "Failed to create ENet server.\n";
            enet_deinitialize();
            return false;
        }

        std::cout << "Server initialized on port " << address.port << "\n";
        return true;
    }

    inline bool Join_Client()
    {
        if (enet_initialize() != 0)
        {
            std::cerr << "An error occurred while initializing ENet.\n";
            return false;
        }
        puts("Initialized ENet");

        client = enet_host_create(nullptr, 1, 2, 0, 0);
        if (client == nullptr)
        {
            std::cerr << "Failed to create ENet client.\n";
            enet_deinitialize();
            return false;
        }

        ENetAddress address;
        enet_address_set_host(&address, "127.0.0.1");
        address.port = 1234;

        clientPeer = enet_host_connect(client, &address, 2, 0);
        if (clientPeer == nullptr)
        {
            std::cerr << "No available peers for initiating connection.\n";
            enet_host_destroy(client);
            enet_deinitialize();
            return false;
        }

        ENetEvent event;
        if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        {
            std::cout << "Connection to server succeeded.\n";
            isConnected = true;
            return true;
        }

        enet_peer_reset(clientPeer);
        std::cerr << "Connection to server failed.\n";
        return false;
    }

    

    inline void Cleanup()
    {
        if (server) {
            enet_host_destroy(server);
            server = nullptr;
            clientConnected = false;
            std::cout << "Server destroyed.\n";
        }

        if (client) {
            enet_host_destroy(client);
            client = nullptr;
            isConnected = false;
            std::cout << "Client destroyed.\n";
        }

        enet_deinitialize();
        std::cout << "ENet deinitialized.\n";
    }
}
