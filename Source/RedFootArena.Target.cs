using UnrealBuildTool;
using System.Collections.Generic;

public class RedFootArenaTarget : TargetRules
{
    public RedFootArenaTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("RedFootArena");
    }
}

