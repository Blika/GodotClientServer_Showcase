#pragma once

#include "../raknet/RakPeerInterface.h"
#include "../raknet/MessageIdentifiers.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace godotserver{
    class PacketHandler;
    class Player;
    class Session;
    class ThreadPool;
    class Server{
		public:

			Server();
			~Server();
			Server(const Server&) = delete;
			Server& operator = (const Server&) = delete;

            static Server* getInstance();
            RakNet::RakPeerInterface* getRakPeerInterface();
            void initThreadpool();
            void shutdownThreadpool();
            ThreadPool* getThreadPool();
            void checkInput();
            void shutdown();
            uint64_t getCurrentTick();
            void run();
            void broadcastPacket(RakNet::BitStream* stream);
            void broadcastPacket(RakNet::BitStream* stream, RakNet::SystemAddress* except);
            bool hasSession(RakNet::SystemAddress* address);
            Session* getSession(RakNet::SystemAddress* address);
            void startSession(RakNet::SystemAddress* address);
            bool closeSession(RakNet::SystemAddress* address);
            bool closeSession(const std::string& address);
            bool hasPlayer(const char* name);
            std::unordered_map<std::string, Player*>* getPlayers();
            void addPlayer(Player* player);
            void removePlayer(Player* player);

        private:
            static Server* instance;
            uint32_t runtimeSleep = 50;
            uint32_t inputSleep = 200;
            int inputThread;
            bool stopped = false;
            RakNet::RakPeerInterface* rakPeer;
            PacketHandler* packetHandler;
            ThreadPool* threadpool = nullptr;
            std::unordered_map<std::string, Session*> sessions{};
            std::unordered_map<std::string, Player*> players{};
            uint64_t tickCounter = 0;
    };
}