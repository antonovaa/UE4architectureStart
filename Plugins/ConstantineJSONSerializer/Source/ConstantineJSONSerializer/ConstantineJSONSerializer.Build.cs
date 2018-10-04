// Some copyright should be here...

using UnrealBuildTool;

public class ConstantineJSONSerializer : ModuleRules
{
	//public ConstantineJSONSerializer(TargetInfo Target)
	public ConstantineJSONSerializer(ReadOnlyTargetRules Target) : base(Target) //+
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"ConstantineJSONSerializer/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ConstantineJSONSerializer/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Json",
                "Sockets",
                "Networking",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
