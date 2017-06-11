// Some copyright should be here...

using UnrealBuildTool;
using System;
using System.IO;

public class MicrosoftSpeechTTS : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	private string MSLIBPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../MSLIB/")); }
	}

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
				"MSLIB",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"Media"

				
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

		LoadMSLIB(Target);

	}

	public bool LoadMSLIB( TargetInfo Target ) {
		bool isLibrarySupported = false;
		string filename = "";
		if((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32)) {
			isLibrarySupported = true;
			if(Target.Platform == UnrealTargetPlatform.Win64) {
				filename = "MSLIB-x64.lib";
			} else if(Target.Platform == UnrealTargetPlatform.Win32) {
				filename = "MSLIB-x86.lib";
			}

			if(filename != "") {
				// path to directory where this .Build.cs is located
				PublicLibraryPaths.Add(MSLIBPath);
				PublicAdditionalLibraries.Add(Path.Combine(MSLIBPath, filename));
				PublicIncludePaths.Add(Path.Combine(MSLIBPath, "TextToSpeech.h"));
				Console.WriteLine("Additional Include dir is: " + Path.Combine(MSLIBPath, "TextToSpeech.h"));
				Definitions.Add(string.Format("WITH_MSLIB={0}", isLibrarySupported ? 1 : 0));
			}
		}
		return isLibrarySupported;
	}
}
