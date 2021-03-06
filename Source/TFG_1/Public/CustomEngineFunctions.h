// Nadir Román Guerrero. All right reserverd

#pragma once

#include "AICharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomEngineFunctions.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API UCustomEngineFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Component|AddComponent")
	static UActorComponent* AddActorComponent(AActor * owner, UClass * ActorComponentClass);

	UFUNCTION(BlueprintCallable, Category = "AI | Calculate walkable point at distance X")
	static FVector CalcWalkablePoint(float MinDistance, float MaxDistance, AActor * Actor, FVector targetPos);

	//static FVector CalcWalkablePointForward(float Distance, FVector Direction, FVector target);
	//static FVector CalcWalkablePointRight(float MinDistance, float MaxDistance, AActor * Actor);
};
