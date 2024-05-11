# GodotClientServer_Showcase
A showcase of a working multiplayer game (to some degree) on [Godot Game Engine](https://godotengine.org/) and [RakNet](https://github.com/facebookarchive/RakNet).

# Server
In this project the server part is written in C++ and uses [RakNet](https://github.com/facebookarchive/RakNet) to handle networking.
For now, it handles creating and closing sessions (basically a class for connections that contains handful functions), managing packets, spawning and syncing player actions across clients (movements are not so nice now, no interpolation implemented).
This server supports multi-threading, meaning it will benefit greatly from multicore cpus in the future. Currently only user input is multi-threaded as there simply isn't much to put on other threads.

Additionally, you can configure some settings in input/settings.json. There you can the limit for incoming connections (in other words players connected) and sleep time for main thread (to get a more consistent tickrate).

# Client
We are using [Godot Game Engine](https://godotengine.org/) as a game client, and [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/index.html) to program in C++, so we don't have to use GDScript (native scripting language in Godot).
For the time being, the client doesn't have anything fancy. Once again it utilizes other cores of your cpu, for now it's just networking.
Currently the game itself only contains one character (a sphere) which you can command to jump (on Space) (that movement will be synced across other clients). If you open multiple clients you can see other "players" in your game, and vice versa.

# Usage
Download the release. Unpack the archive, and run the server first.
> If you run the client first instead, it won't load because it will be waiting until the connection will be established.

Afterwards you can open as much clients as you like.

> Due to collisions, players won't be spawned in a nice way (all clients will tell the server the same spawn position) and therefore will try to push themselves out of each other. They should however end up at different locations eventually.

Alternatively, you can build it yourself. To build the server you'll need `CMake`, and to build the client you'll need `Scons`.
