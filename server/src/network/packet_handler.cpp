#include "packet_handler.hpp"
#include "packets/packets.hpp"
#include "session.hpp"
#include "../raknet/RakSleep.h"
#include "../server/server.hpp"
#include "../player/player.hpp"
#include "../utils/logger.hpp"

namespace godotserver{

    PacketHandler::~PacketHandler(){
        delete packet;
    }

    void PacketHandler::run(){
        RakSleep(30);
        receivePackets();
    }

    void PacketHandler::receivePackets(){
        for(packet = Server::getInstance()->getRakPeerInterface()->Receive(); packet; Server::getInstance()->getRakPeerInterface()->DeallocatePacket(packet), packet = Server::getInstance()->getRakPeerInterface()->Receive()){
            unsigned char packetIdentifier = getPacketIdentifier(packet);
            switch(packetIdentifier){
                case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                    Server::getInstance()->closeSession(&packet->systemAddress);
                    break;
                case ID_REMOTE_CONNECTION_LOST:
                    Server::getInstance()->closeSession(&packet->systemAddress);
                    break;
                case ID_NEW_INCOMING_CONNECTION:
                    Server::getInstance()->startSession(&packet->systemAddress);
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    send_debug("ID_CONNECTION_REQUEST_ACCEPTED");
                    break;
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    send_debug("ID_NO_FREE_INCOMING_CONNECTIONS");
                    break;
                case ID_DISCONNECTION_NOTIFICATION:
                    Server::getInstance()->closeSession(&packet->systemAddress);
                    break;
                case ID_CONNECTION_LOST:
                    Server::getInstance()->closeSession(&packet->systemAddress);
                    break;
                case PLAYER_JOIN:
                    if(!Server::getInstance()->hasSession(&packet->systemAddress)) break;
                    handlePlayerJoin();
                    break;
                case PLAYER_QUIT:
                    if(!Server::getInstance()->hasSession(&packet->systemAddress)) break;
                    handlePlayerQuit();
                    break;
                case PLAYER_SPAWN:
                    //client-side
                    break;
                case PLAYER_DESPAWN:
                    //client-side
                    break;
                case PLAYER_MOVE:
                    if(!Server::getInstance()->hasSession(&packet->systemAddress)) break;
                    handlePlayerMove();
                    break;
                case PLAYER_SEND_TRANSFORM:
                    //client-side
                    break;
                default:
                    send_debug("Packet from ",packet->systemAddress.ToString(),": \n",packet->data);
                    break;
            }
	    }
    }

    void PacketHandler::handlePlayerJoin(){
        PlayerJoin pk = PlayerJoin::decode(packet);
        float transform[]{pk.vx, pk.vy, pk.vz, pk.rx, pk.ry, pk.rz};
        Session* session = Server::getInstance()->getSession(&packet->systemAddress);
        session->createPlayer(pk.name, transform);
        send_debug("Handled PlayerJoin from ",packet->systemAddress.ToString());
    }
    
    void PacketHandler::handlePlayerQuit(){
        PlayerQuit pk = PlayerQuit::decode(packet);;
        Server::getInstance()->getSession(&packet->systemAddress)->destroyPlayer();
        send_debug("Handled PlayerQuit from ",packet->systemAddress.ToString());
    }
    
    void PacketHandler::handlePlayerMove(){
        PlayerMove pk = PlayerMove::decode(packet);
        float diff[]{pk.dvx, pk.dvy, pk.dvz, pk.drx, pk.dry, pk.drz};
        Server::getInstance()->getSession(&packet->systemAddress)->getPlayer()->move(diff[0],diff[1], diff[2]);
        Server::getInstance()->getSession(&packet->systemAddress)->getPlayer()->rotate(diff[3],diff[4], diff[5]);
        //send_debug("Handled PlayerMove from ",packet->systemAddress.ToString());
    }
    
    unsigned char PacketHandler::getPacketIdentifier(RakNet::Packet *p){
        if(p==0) return 255;
        if((unsigned char)p->data[0] == ID_TIMESTAMP){
            RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
            return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
        }else return (unsigned char) p->data[0];
    }

}