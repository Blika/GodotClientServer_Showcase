#include "server.hpp"
#include "../utils/json.hpp"
#include "../utils/logger.hpp"
#include "../raknet/RakSleep.h"
#include "../player/player.hpp"
#include "../network/session.hpp"
#include "../network/packet_handler.hpp"
#include "../utils/threadpool.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

namespace godotserver{

	Server* Server::instance = nullptr;

    Server::Server(){
        Server::instance = this;
        nlohmann::json properties;
	    try{
            std::ifstream mf("input/properties.json");
            properties = nlohmann::json::parse(mf);
        }catch(const std::exception& e){
            throw std::runtime_error("failed to initialize data");
	    }
        runtimeSleep = properties["runtime_sleep"].get<uint32_t>();
        inputSleep = properties["input_sleep"].get<uint32_t>();
        rakPeer = RakNet::RakPeerInterface::GetInstance();
        const char* pswd = "DsjFSH43gja391hgDNdsi391PKDHBZOWEM12";
        rakPeer->SetIncomingPassword(pswd, (int) strlen(pswd));
        RakNet::SocketDescriptor socketDescriptor{};
        socketDescriptor.port = properties["port"].get<int>();
        socketDescriptor.socketFamily = AF_INET;
        if(rakPeer->Startup(properties["max_connections"].get<int>(), &socketDescriptor, 1) != RakNet::RAKNET_STARTED){
			throw std::runtime_error("can't start raknet");
        }
        rakPeer->SetMaximumIncomingConnections(properties["max_connections"].get<int>());
        rakPeer->SetOccasionalPing(true);
        send_debug("Started server at ",rakPeer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, 0).ToString());
        packetHandler = new PacketHandler;
        initThreadpool();
    }

    Server::~Server(){
        if(!stopped) shutdown();
    }

    Server* Server::getInstance(){
        return instance;
    }

    RakNet::RakPeerInterface* Server::getRakPeerInterface(){
        return rakPeer;
    }

    void Server::initThreadpool(){
        threadpool = new ThreadPool;
        threadpool->setThreadCount(std::thread::hardware_concurrency());
        send_debug("Set thread count to ", std::thread::hardware_concurrency());
        inputThread = threadpool->getLeastBusyThread();
        if(inputThread == -1) inputThread = 0;
        threadpool->setInputThread(inputThread);
        threadpool->threads[inputThread]->addJob([=,this] {checkInput();});
    }

    void Server::shutdownThreadpool(){
        //delete threadpool;
        threadpool->wait();
    }

    ThreadPool* Server::getThreadPool(){
        return threadpool;
    }

    int Server::getAvailableThread(){
        if(threadpool == nullptr) return -1;
        return threadpool->getLeastBusyNonInputThread();
    }

    void Server::checkInput(){
        while(true){
            if(stopped) break;
            std::string in;
            std::cin >> in;
            send_debug("command \"",in,"\" does not exist");
            std::this_thread::sleep_for(std::chrono::milliseconds(inputSleep));
        }
    }

    void Server::shutdown(){
        if(stopped) return;
        stopped = true;
        for(auto& [sa, s]: sessions){
            s->disconnect();
            closeSession(sa);
        }
        send_debug("Shutting down raknet...");
        rakPeer->Shutdown(1000);
	    RakNet::RakPeerInterface::DestroyInstance(rakPeer);
        send_debug("Shutting down threadpool...");
        shutdownThreadpool();
        delete packetHandler;
    }

    uint64_t Server::getCurrentTick(){
        return tickCounter;
    }

    void Server::run(){
        while(true){
            if(stopped) break;
            packetHandler->run();
            for(auto&[n,p]: players){
                p->tick(tickCounter);
            }
            tickCounter++;
            std::this_thread::sleep_for(std::chrono::milliseconds(runtimeSleep));
        }
        shutdown();
    }

    void Server::broadcastPacket(RakNet::BitStream* stream){
        for(auto& [sa, s]: sessions){
            s->sendPacket(stream);
        }
    }

    void Server::broadcastPacket(RakNet::BitStream* stream, RakNet::SystemAddress* except){
        for(auto& [sa, s]: sessions){
            if(*s->getSystemAddress() == *except){
                continue;
            }
            s->sendPacket(stream);
        }
    }

    bool Server::hasSession(RakNet::SystemAddress* address){
        return sessions.contains(address->ToString());
    }

    Session* Server::getSession(RakNet::SystemAddress* address){
        if(!hasSession(address)){
            return nullptr;
        }
        return sessions[address->ToString()];
    }

    void Server::startSession(RakNet::SystemAddress* address){
        sessions[address->ToString()] = new Session{address};
    }

    bool Server::closeSession(RakNet::SystemAddress* address){
        if(!hasSession(address)){
            return false;
        }
        delete sessions[address->ToString()];
        sessions.erase(address->ToString());
        return true;
    }

    bool Server::closeSession(std::string address){
        if(!sessions.contains(address)){
            return false;
        }
        delete sessions[address];
        sessions.erase(address);
        return true;
    }

    bool Server::hasPlayer(const char* name){
        return players.contains(name);
    }

    std::unordered_map<std::string, Player*>* Server::getPlayers(){
        return &players;
    }

    void Server::addPlayer(Player* player){
        players[player->getName().C_String()] = player;
    }

    void Server::removePlayer(Player* player){
        if(!hasPlayer(player->getName().C_String())){
            return;
        }
        players.erase(player->getName().C_String());
    }
}