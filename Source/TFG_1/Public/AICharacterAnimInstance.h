
// Nadir Román Guerrero. All right reserverd

#pragma once

#include "Animation/AnimInstance.h"
#include "AICharacter.h"
#include "AICharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API UAICharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	AAICharacter * ownerPawn;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RandomDeathAnimationIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterMovementStatus MovementStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterPosition PositionStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsFiring;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAlerted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMovementType MovementDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsDead;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	void UpdatePosition();
};
