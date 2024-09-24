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
            void receiveConnectionRequestAccepted();
            void handlePlayerJoin(const float& vx, const float& vy, const float& vz, const float& rx, const float& ry, const float& rz, const char* name);
            void handlePlayerMove(const float& vx, const float& vy, const float& vz);
            void handlePlayerRotate(const float& rx, const float& ry, const float& rz);
            void handlePlayerAction(const unsigned char& action);
            bool isSpawned();
            void receivePlayerJoinRespond();
            void receivePlayerAction();
            void receivePlayerMove();
            void receivePlayerRotate();
            void receivePlayerSendTransform();
            void receivePlayerSpawn();
            void receivePlayerDespawn();
        
        private:
            Client* client;
            RakNet::Packet* packet;
            Player* player;
            uint32_t my_id = -1;
            bool spawned = false;
            std::unordered_map<uint32_t, EntityPlayer*> players;
            unsigned char getPacketIdentifier(RakNet::Packet *p);
    };
}