# GodotClientServer_Showcase
A showcase of a working multiplayer game (to some degree) on [Godot Game Engine](https://godotengine.org/) and [RakNet](https://github.com/facebookarchive/RakNet).

# Server
The server part is written in C++ and uses [RakNet](https://github.com/facebookarchive/RakNet) to handle networking.
For now, it handles creating and closing sessions (basically a class for connections that contains handful functions), managing packets, spawning and syncing player actions across clients.

Additionally, you can configure settings in input/settings.json. There you can set the limit for incoming connections (in other words players connected) and sleep time for the main thread (to get a more consistent tickrate).

# Client
We are using [Godot Game Engine](https://godotengine.org/) as a game client, and [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/index.html) to use C++.
Players can move, jump and kick other players. Movement actions use interpolation so they look nice on the client. All player actions are synced.
You can open multiple clients to join the server.

# Client Controls
W - move forward;<br />
S - move backward;<br />
A - move left;<br />
D - move right;<br />
Space = jump;<br />
F = perform a kick;<br />
Left ctrl - lock/unlock mouse cursor.

# Usage
Download from releases. Unpack the archive, and run the server first.
> If you run the client first instead, it won't load because it will be waiting until the connection is established.

Afterwards you can open as much clients as you like.

Alternatively, you can build it yourself. To build the server you'll need `CMake`; to build the client you'll need to export the project (`godot_client/demo`) from `Godot`.
