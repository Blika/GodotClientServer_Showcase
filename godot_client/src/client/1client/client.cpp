#include "client.hpp"
#include "../raknet/RakSleep.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

namespace godot{

    Client::Client(){
        rakPeer = RakNet::RakPeerInterface::GetInstance();
        rakPeer->AllowConnectionResponseIPMigration(false);
        RakNet::SocketDescriptor socketDescriptor{};
        socketDescriptor.port = static_cast<uint32_t>(rnd(1025, 65535));
        socketDescriptor.socketFamily = AF_INET;
        if(rakPeer->Startup(1, &socketDescriptor, 1) != RakNet::RAKNET_STARTED){
			throw std::runtime_error("can't start raknet");
        }
        rakPeer->SetMaximumIncomingConnections(1);
        rakPeer->SetOccasionalPing(true);
        packetHandler = new PacketHandler(this);
        const char* pswd = "DsjFSH43gja391hgDNdsi391PKDHBZOWEM12";
        RakNet::ConnectionAttemptResult car = rakPeer->Connect("127.0.0.1", 8080, pswd, (int) strlen(pswd));
        if(car != RakNet::CONNECTION_ATTEMPT_STARTED){
			throw std::runtime_error("can't connect to server");
        }
    }

    Client::~Client(){
        if(!stopped) shutdown();
    }

    RakNet::RakPeerInterface* Client::getRakPeerInterface(){
        return rakPeer;
    }

    PacketHandler* Client::getPacketHandler(){
        return packetHandler;
    }

	float Client::rnd(float min, float max){
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
        delete rakPeer;
    }

    void Client::run(){
        while(true){
            if(stopped) break;
            packetHandler->run();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        shutdown();
    }

    void Client::sendPacket(RakNet::BitStream* stream){
		rakPeer->Send(stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, server, false);
    }
}