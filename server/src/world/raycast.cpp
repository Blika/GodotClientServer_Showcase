#include "raycast.hpp"
#include "../player/player.hpp"
#include "../server/server.hpp"
#include "../utils/logger.hpp"
#include "transform.hpp"

namespace godotserver{

    RayCast::RayCast(Vector3* s, Vector3* d, const float& dis, const uint32_t& i): start{s}, direction{d}, distance{dis}, id{i}{

    }

    RayCast::~RayCast(){
        delete start;
        delete direction;
    }

    Player* RayCast::getPlayerHit(){
        *direction *= 0.5f;
        Vector3* currentPos = new Vector3(start->x, start->y, start->z);
        uint32_t dist = static_cast<uint32_t>(round(distance));
        Player* ret = nullptr;
        for(uint32_t i = 0; i < dist; i++){
            if(start->distance_to(*currentPos) >= distance){
                break;
            }
            Vector3 end = *currentPos + *direction;
            std::unordered_map<std::string, Player*>* players = Server::getInstance()->getPlayers();
            for(auto& [i,pl]: *players){
                if(pl->getRuntimeId() == id) continue;
                Vector3 collisionCenter = *pl->getTransform()->pos * 1.f;
                if(end.distance_to(collisionCenter) <= 1.5f){
                    ret = pl;
                    break;
                }
            }
            *currentPos = end;
        }
        delete currentPos;
        return ret;
    }

}