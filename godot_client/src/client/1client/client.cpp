#include "client.hpp"
#include "../raknet/RakSleep.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

namespace godot{

    Client::Client(){
    }

    Client::~Client(){
        if(!stopped && started) shutdown();
    }

    void Client::startup(){
        rakPeer = RakNet::RakPeerInterface::GetInstance();
        rakPeer->AllowConnectionResponseIPMigration(false);
        RakNet::SocketDescriptor socketDescriptor{};
        socketDescriptor.port = static_cast<uint32_t>(rnd(1025, 65535));
        socketDescriptor.socketFamily = AF_INET;
        if(rakPeer->Startup(1, &socketDescriptor, 1) != RakNet::RAKNET_STARTED){
            shutdown();
			return;
        }
        rakPeer->SetMaximumIncomingConnections(1);
        rakPeer->SetOccasionalPing(true);
        packetHandler = new PacketHandler(this);
        const char* pswd = "DsjFSH43gja391hgDNdsi391PKDHBZOWEM12";
        RakNet::ConnectionAttemptResult car = rakPeer->Connect("127.0.0.1", 8080, pswd, (int) strlen(pswd));
        if(car != RakNet::CONNECTION_ATTEMPT_STARTED){
            shutdown();
			return;
        }
        started = true;
    }

    RakNet::RakPeerInterface* Client::getRakPeerInterface(){
        return rakPeer;
    }

    PacketHandler* Client::getPacketHandler(){
        return packetHandler;
    }

	float Client::rnd(const float& min, const float& max){
		std::uniform_real_distribution<float> rnd_dist(min,max);
		return rnd_dist(rnd_eng);
	}

    void Client::shutdown(){
        if(stopped) return;
        stopped = true;
        std::cout << "Closing connection..." << '\n';
        rakPeer->CloseConnection(server,true);
        std::cout << "Shutting down raknet..." << '\n';
        rakPeer->Shutdown(1000);
	    RakNet::RakPeerInterface::DestroyInstance(rakPeer);
        delete packetHandler;
    }

    void Client::run(){
        if(!started || stopped){
            return;
        }
        packetHandler->run();
    }

    void Client::sendPacket(RakNet::BitStream* stream){
		rakPeer->Send(stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
    }
}