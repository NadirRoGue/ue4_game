// Nadir Román Guerrero. All right reserverd

#pragma once

#include "GameFramework/WorldSettings.h"
#include "PlayerSettings.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Config=Game)
class UPlayerSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Custom Game Engine Properties")
		int32 ResolutionWidth;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Custom Game Engine Properties")
		int32 ResolutionHeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Custom Game Engine Properties")
		bool UseVSync;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Custom Game Engine Properties")
		bool FullScreenMode;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Custom Game Engine Properties")
		float GameMusicVolume;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Custom Game Engine Properties")
		float GameSoundVolume;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Game Engine Functionality")
		void SavePlayerSettings();
};
