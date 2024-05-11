#include "packet_handler.hpp"
#include "packets/packets.hpp"
#include "../1client/client.hpp"
#include "../../godot/player.h"
#include "../../godot/entity_player.h"
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
                    handleConnectionRequestAccepted();
                    break;
                case ID_CONNECTED_PING:
                case ID_UNCONNECTED_PING:
                    break;
                case PLAYER_SPAWN:
                    handlePlayerSpawn();
                    break;
                case PLAYER_DESPAWN:
                    handlePlayerDespawn();
                    break;
                case PLAYER_SEND_TRANSFORM:
                    handlePlayerSendTransform();
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
    
    void PacketHandler::handleConnectionRequestAccepted(){
        client->server = packet->systemAddress; //remember the server
        client->serverFound = true;
        //std::cout << "ID_CONNECTION_REQUEST_ACCEPTED" << '\n';
    }
    
    void PacketHandler::handlePlayerJoin(float vx, float vy, float vz, float rx, float ry, float rz, const char* name){
        PlayerJoin* pk = new PlayerJoin(RakNet::RakString(name),vx,vy,vz,rx,ry,rz);
        RakNet::BitStream stream;
        pk->encode(&stream);
        client->sendPacket(&stream);
        delete pk;
        //std::cout << "Handled PlayerJoin" << '\n';
    }

    void PacketHandler::handlePlayerMove(float vx, float vy, float vz, float rx, float ry, float rz){
        PlayerMove* pk = new PlayerMove(vx,vy,vz,rx,ry,rz);
        RakNet::BitStream stream;
        pk->encode(&stream);
        client->sendPacket(&stream);
        delete pk;
        //std::cout << "Handled PlayerMove" << '\n';
    }

    void PacketHandler::handlePlayerSendTransform(){
        PlayerSendTransform pk = PlayerSendTransform::decode(packet);
        if(!hasPlayerInMap(pk.id)){
            std::cout << "player does not exist" << '\n';
            return;
        }
        float transform[]{pk.vx, pk.vy, pk.vz, pk.rx, pk.ry, pk.rz};
        players[pk.id]->call_deferred("set_position",Vector3(transform[0],transform[1],transform[2]));
        players[pk.id]->call_deferred("set_rotation",Vector3(transform[3],transform[4],transform[5]));
        std::cout << "Handled PlayerSendTransform" << '\n';
    }

    void PacketHandler::handlePlayerSpawn(){
        PlayerSpawn pk = PlayerSpawn::decode(packet);
        float transform[]{pk.vx, pk.vy, pk.vz, pk.rx, pk.ry, pk.rz};
        if(hasPlayerInMap(pk.id)){
            std::cout << "player does exist" << '\n';
            return;
        }
        Ref<PackedScene> sc = ResourceLoader::get_singleton()->load("res://entity/entity_player.tscn");
        EntityPlayer* pl = (EntityPlayer*) sc->instantiate();
        players[pk.id] = pl;
        player->get_parent()->call_deferred("add_child", pl);
        pl->call_deferred("set_position",Vector3(transform[0],transform[1],transform[2]));
        pl->call_deferred("set_rotation",Vector3(transform[3],transform[4],transform[5]));
        std::cout << "Handled PlayerSpawn" << '\n';
    }

    void PacketHandler::handlePlayerDespawn(){
        PlayerDespawn pk = PlayerDespawn::decode(packet);
        if(!hasPlayerInMap(pk.id)){
            std::cout << "player does not exist" << '\n';
            return;
        }
        players[pk.id]->call_deferred("queue_free");
        players.erase(pk.id);
        std::cout << "Handled PlayerDespawn" << '\n';
    }

    unsigned char PacketHandler::getPacketIdentifier(RakNet::Packet *p){
        if(p==0) return 255;
        if((unsigned char)p->data[0] == ID_TIMESTAMP){
            RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
            return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
        }else return (unsigned char) p->data[0];
    }

}