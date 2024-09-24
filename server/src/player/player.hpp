#pragma once

#include "../raknet/RakString.h"

namespace godotserver{
    class Session;
    class Transform;
    class Vector3;
    class Player{
        public:
            bool hasSpawned = false;
			Player();
			Player(Session* s, const RakNet::RakString& name, float* transform);
			~Player();
			Player(const Player&) = delete;
			Player& operator = (const Player&) = delete;

            Session* getSession();
            RakNet::RakString getName();
            uint32_t getRuntimeId();
            Transform* getTransform();
            void setTransform(Transform* transform);
		    Vector3* getDirectionVector();
            void move(const float& mx, const float& my, const float& mz);
            void rotate(const float& rx, const float& ry, const float& rz);
            void despawnFromAll();
            void spawnToAll();
            void despawnFrom(Session* s);
            void spawnTo(Session* s);
            void sendPlayers();
            void tick(const uint64_t& currentTick);
            bool isCrowdControlled();
            void setRunning(bool v);
            bool isRunning();
            void doKick();
            void doKnockDown();
            void doGetUp();

        private:
            Session* session = nullptr;
            RakNet::RakString name;
            uint32_t id;
            Transform* transform = nullptr;
            uint64_t lastTick = 0;
            bool running = false;
            uint32_t crowd_controlled = 0;
    };
}