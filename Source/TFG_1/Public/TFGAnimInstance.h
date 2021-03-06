// Nadir Román Guerrero. All right reserverd

#pragma once

#include "Animation/AnimInstance.h"
#include "../TFG_1Character.h"
#include "TFGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API UTFGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	ATFG_1Character * ownerPawn;

public:
	UPROPERTY(BlueprintReadOnly)
	class UAnimSequence * IdleAnimation;
	UPROPERTY(BlueprintReadOnly)
	class UAnimSequence * WalkAnimation;
	UPROPERTY(BlueprintReadOnly)
	class UAnimSequence * RunAnimation;
	UPROPERTY(BlueprintReadOnly)
	class UAnimSequence * AimAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsRunning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsWalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsFiring;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsUnarmed;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;
};
