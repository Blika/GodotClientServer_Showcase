#include "session.hpp"
#include "packets/packets.hpp"
#include "../player/player.hpp"
#include "../server/server.hpp"
#include "../utils/logger.hpp"
#include "../world/transform.hpp"

namespace godotserver{

    Session::Session(RakNet::SystemAddress* sa): address{*sa}{
        send_debug("Started new session ",sa->ToString());
    }

    Session::~Session(){
        disconnect();
        send_debug("Closing session ",address.ToString());
    }

    RakNet::SystemAddress* Session::getSystemAddress(){
        return &address;
    }

    bool Session::hasPlayer(){
        return player != nullptr;
    }

    Player* Session::getPlayer(){
        return player;
    }

    void Session::createPlayer(RakNet::RakString name, float transform[]){
        if(player != nullptr){
            send_error(address.ToString()," Player already exists");
            return;
        }
        player = new Player(this, name, transform);
        player->spawnToAll();
        player->sendPlayers();
        Server::getInstance()->addPlayer(player);
        handlePlayerJoinRespond();
        send_debug(address.ToString(), "Created player ",player->getRuntimeId(), " (",name,") at ",transform[0]," ",transform[1]," ",transform[2]);
    }

    void Session::destroyPlayer(){
        if(player == nullptr){
            send_error(address.ToString()," Player does not exist");
            return;
        }
        player->despawnFromAll();
        Server::getInstance()->removePlayer(player);
        send_debug(address.ToString(), "Disposed of player ",player->getRuntimeId());
        delete player;
    }

    void Session::disconnect(){
        destroyPlayer();
        Server::getInstance()->getRakPeerInterface()->CloseConnection(address,true);
    }

    void Session::sendPacket(RakNet::BitStream* stream){
        Server::getInstance()->getRakPeerInterface()->Send(stream, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, address, false);
    }

    void Session::handlePlayerJoinRespond(){
        PlayerJoinRespond* pk = new PlayerJoinRespond(player->getRuntimeId());
        RakNet::BitStream stream;
        pk->encode(&stream);
        sendPacket(&stream);
        delete pk;
    }

    void Session::handleAction(unsigned char action){
        PlayerSendAction* pk = new PlayerSendAction(player->getRuntimeId(),action);
        RakNet::BitStream stream;
        pk->encode(&stream);
        Server::getInstance()->broadcastPacket(&stream, &address);
        delete pk;
    }

    void Session::handleAction(unsigned char action, Session* s){
        PlayerSendAction* pk = new PlayerSendAction(player->getRuntimeId(),action);
        RakNet::BitStream stream;
        pk->encode(&stream);
        s->sendPacket(&stream);
        delete pk;
    }

    void Session::handleRotate(){
        PlayerRotate* pk = new PlayerRotate(player->getRuntimeId(),player->getTransform()->rot->x,player->getTransform()->rot->y,player->getTransform()->rot->z);
        RakNet::BitStream stream;
        pk->encode(&stream);
        Server::getInstance()->broadcastPacket(&stream, &address);
        delete pk;
    }

    void Session::handleMove(float dx, float dy, float dz){
        PlayerMove* pk = new PlayerMove(player->getRuntimeId(),dx,dy,dz);
        RakNet::BitStream stream;
        pk->encode(&stream);
        Server::getInstance()->broadcastPacket(&stream, &address);
        delete pk;
    }

    void Session::handleTransform(){
        PlayerSendTransform* pk = new PlayerSendTransform(player->getRuntimeId(),player->getTransform()->pos->x,player->getTransform()->pos->y,player->getTransform()->pos->z,player->getTransform()->rot->x,player->getTransform()->rot->y,player->getTransform()->rot->z);
        RakNet::BitStream stream;
        pk->encode(&stream);
        Server::getInstance()->broadcastPacket(&stream, &address);
        delete pk;
    }

    void Session::handleSpawn(){
        PlayerSpawn* pk = new PlayerSpawn(player->getRuntimeId(),player->getName(),player->getTransform()->pos->x,player->getTransform()->pos->y,player->getTransform()->pos->z,player->getTransform()->rot->x,player->getTransform()->rot->y,player->getTransform()->rot->z);
        RakNet::BitStream stream;
        pk->encode(&stream);
        Server::getInstance()->broadcastPacket(&stream, &address);
        delete pk;
    }

    void Session::handleSpawn(Session* s){
        PlayerSpawn* pk = new PlayerSpawn(player->getRuntimeId(),player->getName(),player->getTransform()->pos->x,player->getTransform()->pos->y,player->getTransform()->pos->z,player->getTransform()->rot->x,player->getTransform()->rot->y,player->getTransform()->rot->z);
        RakNet::BitStream stream;
        pk->encode(&stream);
        s->sendPacket(&stream);
        delete pk;
    }

    void Session::handleDespawn(){
        PlayerDespawn* pk = new PlayerDespawn(player->getRuntimeId());
        RakNet::BitStream stream;
        pk->encode(&stream);
        Server::getInstance()->broadcastPacket(&stream, &address);
        delete pk;
    }

    void Session::handleDespawn(Session* s){
        PlayerDespawn* pk = new PlayerDespawn(player->getRuntimeId());
        RakNet::BitStream stream;
        pk->encode(&stream);
        s->sendPacket(&stream);
        delete pk;
    }
}