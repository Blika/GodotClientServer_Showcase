#pragma once

#include "../raknet/RakString.h"

namespace godotserver{
    class Session;
    class Transform;
    class Player{
        public:
            bool hasSpawned = false;
			Player();
			Player(Session* s, RakNet::RakString name, float transform[]);
			~Player();
			Player(const Player&) = delete;
			Player& operator = (const Player&) = delete;

            Session* getSession();
            RakNet::RakString getName();
            uint32_t getRuntimeId();
            Transform* getTransform();
            void setTransform(Transform* transform);
            void move(float mx, float my, float mz);
            void rotate(float rx, float ry, float rz);
            void despawnFromAll();
            void spawnToAll();
            void despawnFrom(Session* s);
            void spawnTo(Session* s);
            void sendPlayers();

        private:
            Session* session = nullptr;
            RakNet::RakString name;
            int id;
            Transform* transform = nullptr;
    };
}