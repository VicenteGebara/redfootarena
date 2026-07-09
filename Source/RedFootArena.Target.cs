using UnrealBuildTool;
using System.Collections.Generic;

public class RedFootArenaTarget : TargetRules
{
    public RedFootArenaTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        CppStandard = CppStandardVersion.Cpp20;
        ExtraModuleNames.Add("RedFootArena");
    }
}
