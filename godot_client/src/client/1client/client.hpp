#pragma once

#include "../network/packet_handler.hpp"
#include "../raknet/RakPeerInterface.h"
#include <string>
#include <vector>
#include <random>

namespace godot{
    class Client: public Node{
		public:
            bool serverFound = false;
            RakNet::SystemAddress server;

			Client();
			~Client();
			Client(const Client&) = delete;
			Client& operator = (const Client&) = delete;

            void startup();
            RakNet::RakPeerInterface* getRakPeerInterface();
            PacketHandler* getPacketHandler();
			float rnd(float min, float max);
            void shutdown();
            void run();
            void sendPacket(RakNet::BitStream* stream);

        private:
            bool started = false;
            bool stopped = false;
            int inputThread;
			std::default_random_engine rnd_eng{std::random_device{}()};
            RakNet::RakPeerInterface* rakPeer;
            PacketHandler* packetHandler;
            RakNet::Packet* packet;
    };
}