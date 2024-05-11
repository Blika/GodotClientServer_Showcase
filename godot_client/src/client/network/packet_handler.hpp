#pragma once

#include "raknet/RakPeerInterface.h"
#include <unordered_map>
#include <godot_cpp/classes/node.hpp>

namespace godot{
    class Client;
    class EntityPlayer;
    class Player;
    class PacketHandler: public Node{
        public:
			PacketHandler(Client* c);
			~PacketHandler();
			PacketHandler(const PacketHandler&) = delete;
			PacketHandler& operator = (const PacketHandler&) = delete;

            void run();
            void receivePackets();
            void setPlayer(Player* player);
            bool hasPlayerInMap(uint32_t id);
            void handleConnectionRequestAccepted();
            void handlePlayerJoin(float vx, float vy, float vz, float rx, float ry, float rz, const char* name);
            void handlePlayerMove(float vx, float vy, float vz, float rx, float ry, float rz);
            void handlePlayerSendTransform();
            void handlePlayerSpawn();
            void handlePlayerDespawn();
        
        private:
            Client* client;
            RakNet::Packet* packet;
            Player* player;
            std::unordered_map<uint32_t, EntityPlayer*> players;
            unsigned char getPacketIdentifier(RakNet::Packet *p);
    };
}