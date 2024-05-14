#include "player.hpp"
#include "runtime_id.hpp"
#include "../server/server.hpp"
#include "../network/session.hpp"
#include "../network/packets/player_actions.hpp"
#include "../utils/logger.hpp"
#include "../world/transform.hpp"
#include "../world/raycast.hpp"

namespace godotserver{

    Player::Player(Session* s, RakNet::RakString n, float t[]): session{s}, name{n}{
        id = getNextRuntimeId();
        transform = new Transform(t);
        lastTick = Server::getInstance()->getCurrentTick();
        send_debug("Player ",n," joined the game");
    }

    Player::~Player(){
        send_debug("Player ",name," quit the game");
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

    Vector3* Player::getDirectionVector(){
        Rotation* rot = transform->rot;
        float y = -Math::sin(Math::deg_to_rad(rot->x));
        float xz = Math::cos(Math::deg_to_rad(rot->x));
        float x = -xz * Math::sin(rot->y);
        float z = xz * Math::cos(rot->y);
        Vector3* direction = new Vector3(x,y,z);
        direction->normalize();
        return direction;
    }

    void Player::move(float mx, float my, float mz){
        transform->pos->x += mx;
        transform->pos->y += my;
        transform->pos->z += mz;
        session->handleMove(transform->pos->x,transform->pos->y,transform->pos->z);
    }

    void Player::rotate(float rx, float ry, float rz){
        transform->rot->x = rx;
        transform->rot->y = ry;
        transform->rot->z = rz;
        session->handleRotate();
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
            if(pl->getRuntimeId() == id){
                continue;
            }
            pl->spawnTo(session);
        }
    }

    void Player::tick(uint64_t currentTick){
        uint64_t tickDiff = currentTick - lastTick;
        if(tickDiff < 1){
            return;
        }
        lastTick = currentTick;
        if(crowd_controlled > 0){
            crowd_controlled -= tickDiff;
            if(crowd_controlled <= 0){
                doGetUp();
            }
        }
    }

    bool Player::isCrowdControlled(){
        return crowd_controlled > 0;
    }
    void Player::setRunning(bool v){
        running = v;
        session->handleAction(v ? ACTION_START_SPRINT : ACTION_STOP_SPRINT);
    }
    bool Player::isRunning(){
        return running;
    }
    void Player::doKick(){
        session->handleAction(ACTION_KICK);
        session->handleAction(ACTION_KICK, session);
        RayCast* raycast = new RayCast(new Vector3(transform->pos->x,transform->pos->y,transform->pos->z), getDirectionVector(), 5, id);
        Player* hit = raycast->getPlayerHit();
        if(hit != nullptr){
            hit->doKnockDown();
        }
    }
    void Player::doKnockDown(){
        if(crowd_controlled > 0) return;
        crowd_controlled = 40;
        session->handleAction(ACTION_KNOCK_DOWN);
        session->handleAction(ACTION_KNOCK_DOWN, session);
    }
    void Player::doGetUp(){
        session->handleAction(ACTION_GET_UP);
        session->handleAction(ACTION_GET_UP, session);
    }
}