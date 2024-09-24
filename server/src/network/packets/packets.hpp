#pragma once

#include <stdint.h>
#include "player_actions.hpp"
#include "../../raknet/BitStream.h"
#include "../../raknet/RakString.h"
#include "../../raknet/MessageIdentifiers.h"

namespace godotserver{
    class Packet{
        public:
            unsigned char typeId = ID_CONNECTED_PING;
            Packet(){
            }
            ~Packet(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
            }
            static Packet decode(RakNet::Packet* packet){
                return Packet();
            }
    };
    class PlayerJoin: public Packet{
        public:
            unsigned char typeId = PLAYER_JOIN;
            RakNet::RakString name;
            float vx;
            float vy;
            float vz;
            float rx;
            float ry;
            float rz;
            PlayerJoin(const RakNet::RakString& n, const float& vxx, const float& vyy, const float& vzz, const float& rxx, const float& ryy, const float& rzz): name{n}, vx{vxx},vy{vyy},vz{vzz},rx{rxx},ry{ryy},rz{rzz}{
            }
            ~PlayerJoin(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(name);
                stream->Write(vx);
                stream->Write(vy);
                stream->Write(vz);
                stream->Write(rx);
                stream->Write(ry);
                stream->Write(rz);
            }
            static PlayerJoin decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char id;
                RakNet::RakString n;
                float vx,vy,vz,rx,ry,rz;
                bitStream.Read(id);
                bitStream.Read(n);
                bitStream.Read(vx);
                bitStream.Read(vy);
                bitStream.Read(vz);
                bitStream.Read(rx);
                bitStream.Read(ry);
                bitStream.Read(rz);
                return PlayerJoin(n,vx,vy,vz,rx,ry,rz);
            }
    };
    class PlayerQuit: public Packet{
        public:
            PlayerQuit(){
            }
            ~PlayerQuit(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
            }
            static PlayerQuit decode(RakNet::Packet* packet){
                return PlayerQuit();
            }

        private:
            unsigned char typeId = PLAYER_QUIT;
    };
    class PlayerSpawn: public Packet{
        public:
            unsigned char typeId = PLAYER_SPAWN;
            uint32_t id;
            RakNet::RakString name;
            float vx;
            float vy;
            float vz;
            float rx;
            float ry;
            float rz;
            PlayerSpawn(const uint32_t& i, const RakNet::RakString& n, const float& vxx, const float& vyy, const float& vzz, const float& rxx, const float& ryy, const float& rzz): id{i}, name{n}, vx{vxx},vy{vyy},vz{vzz},rx{rxx},ry{ryy},rz{rzz}{
            }
            ~PlayerSpawn(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
                stream->Write(name);
                stream->Write(vx);
                stream->Write(vy);
                stream->Write(vz);
                stream->Write(rx);
                stream->Write(ry);
                stream->Write(rz);
            }
            static PlayerSpawn decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                uint32_t id;
                RakNet::RakString n;
                float vx,vy,vz,rx,ry,rz;
                bitStream.Read(tid);
                bitStream.Read(id);
                bitStream.Read(n);
                bitStream.Read(vx);
                bitStream.Read(vy);
                bitStream.Read(vz);
                bitStream.Read(rx);
                bitStream.Read(ry);
                bitStream.Read(rz);
                return PlayerSpawn(id,n,vx,vy,vz,rx,ry,rz);
            }
    };
    class PlayerDespawn: public Packet{
        public:
            unsigned char typeId = PLAYER_DESPAWN;
            uint32_t id;
            PlayerDespawn(uint32_t i): id{i}{
            }
            ~PlayerDespawn(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
            }
            static PlayerDespawn decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                uint32_t id;
                bitStream.Read(tid);
                bitStream.Read(id);
                return PlayerDespawn(id);
            }
    };
    class PlayerMove: public Packet{
        public:
            unsigned char typeId = PLAYER_MOVE;
            uint32_t id;
            float dvx;
            float dvy;
            float dvz;
            PlayerMove(const uint32_t& i, const float& vxx, const float& vyy, const float& vzz):  id{i},dvx{vxx},dvy{vyy},dvz{vzz}{
            }
            ~PlayerMove(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
                stream->Write(dvx);
                stream->Write(dvy);
                stream->Write(dvz);
            }
            static PlayerMove decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                uint32_t id;
                float dvx,dvy,dvz;
                bitStream.Read(tid);
                bitStream.Read(id);
                bitStream.Read(dvx);
                bitStream.Read(dvy);
                bitStream.Read(dvz);
                return PlayerMove(id,dvx,dvy,dvz);
            }
    };
    class PlayerRotate: public Packet{
        public:
            unsigned char typeId = PLAYER_ROTATE;
            uint32_t id;
            float rx;
            float ry;
            float rz;
            PlayerRotate(const uint32_t& i, const float& rxx, const float& ryy, const float& rzz): id{i},rx{rxx},ry{ryy},rz{rzz}{
            }
            ~PlayerRotate(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
                stream->Write(rx);
                stream->Write(ry);
                stream->Write(rz);
            }
            static PlayerRotate decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                uint32_t id;
                float rx,ry,rz;
                bitStream.Read(tid);
                bitStream.Read(id);
                bitStream.Read(rx);
                bitStream.Read(ry);
                bitStream.Read(rz);
                return PlayerRotate(id,rx,ry,rz);
            }
    };
    class PlayerSendTransform: public Packet{
        public:
            unsigned char typeId = PLAYER_SEND_TRANSFORM;
            uint32_t id;
            float vx;
            float vy;
            float vz;
            float rx;
            float ry;
            float rz;
            PlayerSendTransform(const uint32_t& i, const float& vxx, const float& vyy, const float& vzz, const float& rxx, const float& ryy, const float& rzz): id{i}, vx{vxx},vy{vyy},vz{vzz},rx{rxx},ry{ryy},rz{rzz}{
            }
            ~PlayerSendTransform(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
                stream->Write(vx);
                stream->Write(vy);
                stream->Write(vz);
                stream->Write(rx);
                stream->Write(ry);
                stream->Write(rz);
            }
            static PlayerSendTransform decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                uint32_t id;
                float vx,vy,vz,rx,ry,rz;
                bitStream.Read(tid);
                bitStream.Read(id);
                bitStream.Read(vx);
                bitStream.Read(vy);
                bitStream.Read(vz);
                bitStream.Read(rx);
                bitStream.Read(ry);
                bitStream.Read(rz);
                return PlayerSendTransform(id,vx,vy,vz,rx,ry,rz);
            }
    };
    class PlayerSendAction: public Packet{
        public:
            unsigned char typeId = PLAYER_SEND_ACTION;
            uint32_t id;
            unsigned char action = ACTION_NONE;
            PlayerSendAction(const uint32_t& i, const unsigned char& act): id{i}, action{act}{
            }
            ~PlayerSendAction(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
                stream->Write(action);
            }
            static PlayerSendAction decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid, action;
                uint32_t id;
                bitStream.Read(tid);
                bitStream.Read(id);
                bitStream.Read(action);
                return PlayerSendAction(id,action);
            }
    };
    class PlayerJoinRespond: public Packet{
        public:
            unsigned char typeId = PLAYER_JOIN_RESPOND;
            uint32_t id;
            PlayerJoinRespond(const uint32_t& i): id{i}{
            }
            ~PlayerJoinRespond(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(id);
            }
            static PlayerJoinRespond decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                uint32_t id;
                bitStream.Read(tid);
                bitStream.Read(id);
                return PlayerJoinRespond(id);
            }
    };
}