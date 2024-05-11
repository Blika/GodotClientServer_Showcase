#include "player.hpp"
#include "runtime_id.hpp"
#include "../server/server.hpp"
#include "../network/session.hpp"
#include "../utils/logger.hpp"
#include "../world/transform.hpp"

namespace godotserver{

    Player::Player(Session* s, RakNet::RakString n, float t[]): session{s}, name{n}{
        id = getNextRuntimeId();
        transform = new Transform(t);
    }

    Player::~Player(){
        delete transform;
    }

    Session* Player::getSession(){
        return session;
    }

    RakNet::RakString Player::getName(){
        return name;
    }

    uint32_t Player::getRuntimeId(){
        return id;
    }

    Transform* Player::getTransform(){
        return transform;
    }

    void Player::setTransform(Transform* t){
        transform = t;
        session->handleTransform();
    }

    void Player::move(float mx, float my, float mz){
        transform->pos->x += mx;
        transform->pos->y += my;
        transform->pos->z += mz;
        session->handleTransform();
    }

    void Player::rotate(float rx, float ry, float rz){
        transform->rot->x += rx;
        transform->rot->y += ry;
        transform->rot->z += rz;
        session->handleTransform();
    }

    void Player::spawnToAll(){
        session->handleSpawn();
        hasSpawned = true;
    }

    void Player::despawnFromAll(){
        session->handleDespawn();
        hasSpawned = false;
    }

    void Player::spawnTo(Session* s){
        session->handleSpawn(s);
    }

    void Player::despawnFrom(Session* s){
        session->handleDespawn(s);
    }

    void Player::sendPlayers(){
        std::unordered_map<std::string, Player*>* players = Server::getInstance()->getPlayers();
        for(auto& [i,pl]: *players){
            if(pl->getRuntimeId() == id) continue;
            pl->spawnTo(session);
        }
    }

}