#pragma once

#include "../raknet/RakPeerInterface.h"
#include "../raknet/RakString.h"
#include <iostream>
#include <string>


namespace godotserver{

    class Player;

    class Session{
		public:
			Session(RakNet::SystemAddress* sa);
			~Session();
			Session(const Session&) = delete;
			Session& operator = (const Session&) = delete;

            RakNet::SystemAddress* getSystemAddress();
            bool hasPlayer();
            Player* getPlayer();
            void createPlayer(RakNet::RakString name, float trasnform[]);
            void destroyPlayer();
            void disconnect();
            void sendPacket(RakNet::BitStream* stream);

            void handleTransform();
            void handleSpawn();
            void handleSpawn(Session* s);
            void handleDespawn();
            void handleDespawn(Session* s);

        private:
            RakNet::SystemAddress address;
            Player* player = nullptr;
    };

}