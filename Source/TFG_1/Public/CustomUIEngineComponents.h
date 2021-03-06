// Nadir Román Guerrero. All right reserverd

#pragma once

#include "MyWorldSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomUIEngineComponents.generated.h"

USTRUCT(BlueprintType)
struct FResolutionStruct
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	FResolutionStruct()
	{
		Width = Height = 0;
	}
};

/**
 * 
 */
UCLASS()
class TFG_1_API UCustomUIEngineComponents : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UMyWorldSettings * INSTANCE;

	UFUNCTION(BlueprintCallable, Category = "TheUkranianConflictLibrary")
	static FResolutionStruct SwitchResolution(FResolutionStruct res, bool decrease);

	UFUNCTION(BlueprintCallable, Category = "TheUkranianConflictLibrary")
	static void UpdateInputSettings();

	UFUNCTION(BlueprintCallable, Category = "TheUkranianConflictLibrary")
	static UMyWorldSettings * GetWorldSettings(bool &IsValid);

	UFUNCTION(BlueprintCallable, Category = "TheUkranianConflictLibrary")
	static UObject * CreateObjectFromClass(UObject * outer, UClass * uClass);
};
