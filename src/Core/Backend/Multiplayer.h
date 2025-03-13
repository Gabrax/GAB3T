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

        ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
        if (peer == nullptr)
        {
            std::cerr << "No available peers for initiating connection.\n";
            enet_host_destroy(client);
            enet_deinitialize();
            return false;
        }

        ENetEvent event;
        int retries = 5;  // Retry up to 5 times
        while (retries-- > 0) {
            if (enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
            {
                std::cout << "Connection to server succeeded.\n";
                return true;
            }
            std::cout << "Retrying connection... (" << retries << " attempts left)\n";
        }

        enet_peer_reset(peer);
        std::cerr << "Connection to server failed after multiple attempts.\n";
        return false;
    }

    inline void Update_Host()
    {
        if (!server) return;

        ENetEvent event;
        while (enet_host_service(server, &event, 0) > 0)  // Non-blocking check for events
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "Client connected from " << event.peer->address.host << ":" << event.peer->address.port << "\n";
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Received packet from client.\n";
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Client disconnected.\n";
                    break;

                default:
                    break;
            }
        }
    }

    inline void Update_Client()
    {
        if (!client) return;

        ENetEvent event;
        while (enet_host_service(client, &event, 0) > 0) // Non-blocking check for events
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Received packet from server.\n";
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server.\n";
                    isConnected = false; // Mark the client as disconnected
                    break;

                default:
                    break;
            }
        }
    }

    inline void Cleanup()
    {
        if (server) {
            enet_host_destroy(server);
            server = nullptr;
            std::cout << "Server destroyed.\n";
        }

        if (client) {
            enet_host_destroy(client);
            client = nullptr;
            std::cout << "Client destroyed.\n";
        }

        enet_deinitialize();
        std::cout << "ENet deinitialized.\n";
    }
}
