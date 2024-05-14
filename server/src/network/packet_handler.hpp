#pragma once

#include "../raknet/RakPeerInterface.h"

namespace godotserver{
    class PacketHandler{
        public:
			PacketHandler(){

            }
			~PacketHandler();
			PacketHandler(const PacketHandler&) = delete;
			PacketHandler& operator = (const PacketHandler&) = delete;

            void run();
            void receivePackets();
            void handlePlayerAction();
            void handlePlayerJoin();
            void handlePlayerQuit();
            void handlePlayerMove();
            void handlePlayerRotate();
        
        private:
            RakNet::Packet* packet;
            unsigned char getPacketIdentifier(RakNet::Packet *p);
    };
}