// Nadir Román Guerrero. All right reserverd

#pragma once

#include "Object.h"
#include "GameCharacter.h"
#include "KnownEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API UKnownEnemy : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	AGameCharacter * Character;

	UPROPERTY(BlueprintReadOnly)
	FVector LastKnownLocation;

	void SetCharacter(AGameCharacter * enemy);
	void Update();
};
