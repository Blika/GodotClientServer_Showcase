#pragma once

#include <stdint.h>
#include "../../raknet/BitStream.h"
#include "../../raknet/RakString.h"
#include "../../raknet/MessageIdentifiers.h"

namespace godot{
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
            PlayerJoin(RakNet::RakString n, float vxx, float vyy, float vzz, float rxx, float ryy, float rzz): name{n}, vx{vxx},vy{vyy},vz{vzz},rx{rxx},ry{ryy},rz{rzz}{
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
            PlayerSpawn(uint32_t i, RakNet::RakString n, float vxx, float vyy, float vzz, float rxx, float ryy, float rzz): id{i}, name{n}, vx{vxx},vy{vyy},vz{vzz},rx{rxx},ry{ryy},rz{rzz}{
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
            float dvx;
            float dvy;
            float dvz;
            float drx;
            float dry;
            float drz;
            PlayerMove(float vxx, float vyy, float vzz, float rxx, float ryy, float rzz): dvx{vxx},dvy{vyy},dvz{vzz},drx{rxx},dry{ryy},drz{rzz}{
            }
            ~PlayerMove(){
            }
            void encode(RakNet::BitStream* stream){
                stream->Write(typeId);
                stream->Write(dvx);
                stream->Write(dvy);
                stream->Write(dvz);
                stream->Write(drx);
                stream->Write(dry);
                stream->Write(drz);
            }
            static PlayerMove decode(RakNet::Packet* packet){
                RakNet::BitStream bitStream(packet->data, packet->length, false);
                unsigned char tid;
                float dvx,dvy,dvz,drx,dry,drz;
                bitStream.Read(tid);
                bitStream.Read(dvx);
                bitStream.Read(dvy);
                bitStream.Read(dvz);
                bitStream.Read(drx);
                bitStream.Read(dry);
                bitStream.Read(drz);
                return PlayerMove(dvx,dvy,dvz,drx,dry,drz);
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
            PlayerSendTransform(uint32_t i, float vxx, float vyy, float vzz, float rxx, float ryy, float rzz): id{i}, vx{vxx},vy{vyy},vz{vzz},rx{rxx},ry{ryy},rz{rzz}{
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
}