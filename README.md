# RedFoot Arena

RedFoot Arena is an arcade 3D football game prototype built with Unreal Engine.

Current focus: offline MVP only.

Implemented architecture:

- third-person player character;
- arcade movement with sprint and stamina;
- physical ball with kick and pass impulses;
- two goals with goal detection;
- score and match timer managers;
- reset after goals;
- simple generated arena;
- basic opponent bot controller.

Out of scope for now:

- online multiplayer;
- career mode;
- store, coins or skins;
- multiple arenas;
- rankings, seasons or transfers.

The code is organized so later phases can add 4v4 matches, career progression,
goalkeepers, line players, different arenas, cosmetics and multiplayer without
turning the MVP classes into one large system.
