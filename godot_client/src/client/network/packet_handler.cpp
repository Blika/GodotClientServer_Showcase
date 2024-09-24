#include "packet_handler.hpp"
#include "packets/packets.hpp"
#include "../1client/client.hpp"
#include "../../godot/player.hpp"
#include "../../godot/model.hpp"
#include "../../godot/entity_player.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "../raknet/RakSleep.h"

namespace godot{

    PacketHandler::PacketHandler(Client* c): client{c}{
        
    }

    PacketHandler::~PacketHandler(){
        delete packet;
    }

    void PacketHandler::run(){
        receivePackets();
    }

    void PacketHandler::receivePackets(){
        for(packet = client->getRakPeerInterface()->Receive(); packet; client->getRakPeerInterface()->DeallocatePacket(packet), packet = client->getRakPeerInterface()->Receive()){
            unsigned char packetIdentifier = getPacketIdentifier(packet);
            switch(packetIdentifier){
                case ID_DISCONNECTION_NOTIFICATION:
                    std::cout << "ID_DISCONNECTION_NOTIFICATION" << '\n';
                    break;
                case ID_ALREADY_CONNECTED:
                    std::cout << "ID_ALREADY_CONNECTED with guid " << packet->guid.ToString() << '\n';
                    break;
                case ID_INCOMPATIBLE_PROTOCOL_VERSION:
                    std::cout << "ID_INCOMPATIBLE_PROTOCOL_VERSION" << '\n';
                    break;
                case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                    std::cout << "ID_REMOTE_DISCONNECTION_NOTIFICATION" << '\n'; 
                    break;
                case ID_REMOTE_CONNECTION_LOST:
                    std::cout << "ID_REMOTE_CONNECTION_LOST" << '\n';
                    break;
                case ID_REMOTE_NEW_INCOMING_CONNECTION:
                    std::cout << "ID_REMOTE_NEW_INCOMING_CONNECTION" << '\n';
                    break;
                case ID_CONNECTION_BANNED:
                    std::cout << "ID_CONNECTION_BANNED" << '\n';
                    break;			
                case ID_CONNECTION_ATTEMPT_FAILED:
                    std::cout << "ID_CONNECTION_ATTEMPT_FAILED" << '\n';
                    break;
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    std::cout << "ID_NO_FREE_INCOMING_CONNECTIONS" << '\n';
                    break;
                case ID_INVALID_PASSWORD:
                    std::cout << "ID_INVALID_PASSWORD"<< '\n';
                    break;
                case ID_CONNECTION_LOST:
                    std::cout << "ID_CONNECTION_LOST" << '\n';
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    //receiveConnectionRequestAccepted();
                    break;
                case ID_CONNECTED_PING:
                case ID_UNCONNECTED_PING:
                    break;
                case PLAYER_SPAWN:
                    receivePlayerSpawn();
                    break;
                case PLAYER_DESPAWN:
                    receivePlayerDespawn();
                    break;
                case PLAYER_SEND_TRANSFORM:
                    receivePlayerSendTransform();
                    break;
                case PLAYER_ROTATE:
                    receivePlayerRotate();
                    break;
                case PLAYER_MOVE:
                    receivePlayerMove();
                    break;
                case PLAYER_SEND_ACTION:
                    receivePlayerAction();
                    break;
                case PLAYER_JOIN_RESPOND:
                    receivePlayerJoinRespond();
                    break;
                default:
                    std::cout << packet->data << '\n';
                    break;
            }
	    }
    }

    void PacketHandler::setPlayer(Player* p){
        player = p;
    }

    bool PacketHandler::hasPlayerInMap(uint32_t id){
        if(players.find(id) != players.end()){
            return true;
        }
        return false;
    }
    
    void PacketHandler::receiveConnectionRequestAccepted(){
        client->server = packet->systemAddress;
        client->serverFound = true;
    }

    void PacketHandler::handlePlayerJoin(const float& vx, const float& vy, const float& vz, const float& rx, const float& ry, const float& rz, const char* name){
        PlayerJoin* pk = new PlayerJoin(RakNet::RakString(name),vx,vy,vz,rx,ry,rz);
        RakNet::BitStream stream;
        pk->encode(&stream);
        client->sendPacket(&stream);
        delete pk;
    }

    void PacketHandler::handlePlayerMove(const float& vx, const float& vy, const float& vz){
        PlayerMove* pk = new PlayerMove(my_id, vx,vy,vz);
        RakNet::BitStream stream;
        pk->encode(&stream);
        client->sendPacket(&stream);
        delete pk;
    }

    void PacketHandler::handlePlayerRotate(const float& rx, const float& ry, const float& rz){
        PlayerRotate* pk = new PlayerRotate(my_id, rx,ry,rz);
        RakNet::BitStream stream;
        pk->encode(&stream);
        client->sendPacket(&stream);
        delete pk;
    }

    void PacketHandler::handlePlayerAction(const unsigned char& action){
        PlayerSendAction* pk = new PlayerSendAction(my_id,action);
        RakNet::BitStream stream;
        pk->encode(&stream);
        client->sendPacket(&stream);
        delete pk;
    }

    bool PacketHandler::isSpawned(){
        return spawned;
    }

    void PacketHandler::receivePlayerJoinRespond(){
        PlayerJoinRespond pk = PlayerJoinRespond::decode(packet);
        my_id = pk.id;
        spawned = true;
    }

    void PacketHandler::receivePlayerAction(){
        PlayerSendAction pk = PlayerSendAction::decode(packet);
        if(!hasPlayerInMap(pk.id) && pk.id != my_id){
            std::cout << "player does not exist6" << '\n';
            return;
        }
        if(pk.id == my_id){
            switch(pk.action){
                case ACTION_KICK:
                    player->playKickAnimation();
                    break;
                case ACTION_KNOCK_DOWN:
                    player->playKnockDownAnimation();
                    break;
                case ACTION_GET_UP:
                    player->playGetUpAnimation();
                    break;
            }
        }else{
            switch(pk.action){
                case ACTION_KICK:
                    players[pk.id]->playKickAnimation();
                    break;
                case ACTION_KNOCK_DOWN:
                    players[pk.id]->playKnockDownAnimation();
                    break;
                case ACTION_GET_UP:
                    players[pk.id]->playGetUpAnimation();
                    break;
                case ACTION_START_SPRINT:
                    players[pk.id]->setRunning(true);
                    break;
                case ACTION_STOP_SPRINT:
                    players[pk.id]->setRunning(false);
                    break;
            }
        }
    }

    void PacketHandler::receivePlayerMove(){
        PlayerMove pk = PlayerMove::decode(packet);
        if(!hasPlayerInMap(pk.id)){
            std::cout << "player does not exist3" << '\n';
            return;
        }
        float transform[]{pk.dvx, pk.dvy, pk.dvz};
        players[pk.id]->addPosDestination(Vector3(transform[0],transform[1]+0.7f,transform[2]));
    }

    void PacketHandler::receivePlayerRotate(){
        PlayerRotate pk = PlayerRotate::decode(packet);
        if(!hasPlayerInMap(pk.id) && pk.id != my_id){
            std::cout << "player does not exist2" << '\n';
            return;
        }
        float transform[]{pk.rx, pk.ry, pk.rz};
        if(pk.id == my_id){
            player->set_rotation(Vector3(transform[0],transform[1],transform[2]));
        }else{
            players[pk.id]->addRotDestination(Vector3(transform[0],transform[1],transform[2]));
            //players[pk.id]->call_deferred("set_rotation",Vector3(transform[0],transform[1],transform[2]));
        }
    }

    void PacketHandler::receivePlayerSendTransform(){
        PlayerSendTransform pk = PlayerSendTransform::decode(packet);
        if(!hasPlayerInMap(pk.id) && pk.id != my_id){
            std::cout << "player does not exist1" << '\n';
            return;
        }
        float transform[]{pk.vx, pk.vy, pk.vz, pk.rx, pk.ry, pk.rz};
        if(pk.id == my_id){
            player->set_position(Vector3(transform[0],transform[1],transform[2]));
            player->getModel()->set_rotation(Vector3(transform[3],transform[4],transform[5]));
        }else{
            players[pk.id]->addPosDestination(Vector3(transform[0],transform[1]+0.7f,transform[2]));
            players[pk.id]->addRotDestination(Vector3(transform[3],transform[4],transform[5]));

            /*players[pk.id]->call_deferred("set_position",Vector3(transform[0],transform[1],transform[2]));
            players[pk.id]->call_deferred("set_rotation",Vector3(transform[3],transform[4],transform[5]));*/
        }
    }

    void PacketHandler::receivePlayerSpawn(){
        PlayerSpawn pk = PlayerSpawn::decode(packet);
        float transform[]{pk.vx, pk.vy, pk.vz, pk.rx, pk.ry, pk.rz};
        if(hasPlayerInMap(pk.id)){
            std::cout << "player does exist4" << '\n';
            return;
        }
        Ref<PackedScene> sc = ResourceLoader::get_singleton()->load("res://entity/entity_player.tscn");
        EntityPlayer* pl = (EntityPlayer*) sc->instantiate();
        players[pk.id] = pl;
        player->get_parent()->add_child(pl);
        pl->set_position(Vector3(transform[0],transform[1]+0.7f,transform[2]));
        pl->set_rotation(Vector3(transform[3],transform[4],transform[5]));
    }

    void PacketHandler::receivePlayerDespawn(){
        PlayerDespawn pk = PlayerDespawn::decode(packet);
        if(!hasPlayerInMap(pk.id)){
            std::cout << "player does not exist5" << '\n';
            return;
        }
        players[pk.id]->queue_free();
        players.erase(pk.id);
    }

    unsigned char PacketHandler::getPacketIdentifier(RakNet::Packet *p){
        if(p==0) return 255;
        if((unsigned char)p->data[0] == ID_TIMESTAMP){
            RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
            return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
        }else return (unsigned char) p->data[0];
    }

}