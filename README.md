# ReplaySystemUE5Port
 
Add the following to the DefaultEngine.ini

[/Script/Engine.GameEngine]
!NetDriverDefinitions=ClearArray
+NetDriverDefinitions=(DefName="DemoNetDriver",DriverClassName="/Script/Engine.DemoNetDriver",DriverClassNameFallback="/Script/Engine.DemoNetDriver")

In order to be able to use the replay system, put all assets that should be included in the replay on Repicated, 
and add ReplaySystemAdvanced_PC in your gamamode under Replay Spectator ClassAdd the following to the DefaultEngine.ini

note from byTiny: tbh I have no idea if this works but it loads in unreal 5.
