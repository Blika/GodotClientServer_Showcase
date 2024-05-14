#pragma once

#include <stdint.h>

namespace godotserver{
    class Player;
    class Vector3;
    class RayCast{

        public:
            RayCast(Vector3* start, Vector3* direction, float distance, uint32_t id);
            ~RayCast();
			RayCast(const RayCast&) = delete;
			RayCast& operator = (const RayCast&) = delete;

            Player* getPlayerHit();

        private:
            Vector3* start;
            Vector3* direction;
            float distance;
            uint32_t id;
    };
}