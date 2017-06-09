// Some copyright should be here...

using UnrealBuildTool;
using System;
using System.IO;

public class MicrosoftSpeechTTS : ModuleRules
{
	public MicrosoftSpeechTTS(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"MicrosoftSpeechTTS/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"MicrosoftSpeechTTS/Private",
				"dlls",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
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

		
		if((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32)) {

			string filename = "sapi.lib";
			// path to directory where this .Build.cs is located
			string BaseDir = ModuleDirectory;
			PublicLibraryPaths.Add(Path.Combine(BaseDir, "../dlls"));
			PrivateIncludePaths.Add("dlls/sapi.h");
			PrivateIncludePaths.Add("dlls/sapiddk.h");
			PrivateIncludePaths.Add("dlls/sperror.h");
			PrivateIncludePaths.Add("dlls/sphelper.h");
			Definitions.Add(string.Format("WITH_SAPI={0}", 1));
		
		}

	}
}
