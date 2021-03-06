// Nadir Román Guerrero. All right reserverd

#pragma once

#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

UCLASS()
class TFG_1_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString ID;

	UPROPERTY(BlueprintReadWrite, Category = "Character Status")
	int32 HP;

	UPROPERTY(BlueprintReadWrite, Category = "Character Status")
	int32 TeamId;

	UPROPERTY(BlueprintReadWrite)
	bool IsDead;

	UPROPERTY(BlueprintReadWrite)
	AGameCharacter * LastHarmer;

	// Sets default values for this character's properties
	AGameCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision Events")
	void OnProjectileCollision(FName HitBone);
};
