# RedFoot Arena

RedFoot Arena is an arcade 3D football game prototype built with Unreal Engine.

Current focus: offline MVP only.

## Local build requirements

- Unreal Engine 5.8.
- Visual Studio 2022 Build Tools with the C++ toolchain.
- Windows SDK 10.0.19041.0 or newer.

Implemented architecture:

- third-person player character;
- arcade movement with sprint and stamina;
- physical ball with kick and pass impulses;
- two goals with goal detection;
- score and match timer managers;
- reset after goals;
- simple generated arena;
- basic opponent bot controller.

Latest MVP loop improvements:

- clearer HUD with score, timer, stamina and goal restart feedback;
- procedural arena colors and simple field markings;
- colored player and opponent bodies;
- small kick/pass cooldown for more predictable ball control;
- bot approach logic that tries to line up behind the ball before shooting.
- kickoff delay before play starts or restarts;
- automatic ball recovery when it falls or leaves the playable area;
- manual play reset for quick local testing.

Latest visual pass:

- procedural player bodies now include head, team ring and forward marker;
- home and away colors are more distinct on players and goals;
- ball has simple arcade stripe details;
- match lighting and third-person camera were tuned for better readability;
- arena remains fully procedural while looking more like a playable test court.

Latest asset pass:

- selected free CC0 models from Kenney were downloaded and organized;
- imported Unreal assets live under `Content/External/Kenney`;
- source FBX files and license files live under `ExternalAssets/Kenney`;
- arena now uses optional decorative props such as banners, columns, gates, trophies and team flags;
- asset origins and licenses are documented in `Docs/ThirdPartyAssets.md`.

Latest audio pass:

- basic kick and pass feedback sounds;
- goal, kickoff, reset and match-end audio cues;
- sounds use engine-provided assets for now, so no manual import step is needed.

Current test controls:

- WASD: move;
- Mouse: camera;
- Left Shift: sprint;
- Space or left mouse button: kick;
- E or right mouse button: pass;
- R: reset the current play to kickoff.

Out of scope for now:

- online multiplayer;
- career mode;
- store, coins or skins;
- multiple arenas;
- rankings, seasons or transfers.

The code is organized so later phases can add 4v4 matches, career progression,
goalkeepers, line players, different arenas, cosmetics and multiplayer without
turning the MVP classes into one large system.
