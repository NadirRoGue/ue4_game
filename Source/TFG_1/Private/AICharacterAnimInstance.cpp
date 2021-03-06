// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "AICharacterAnimInstance.h"

void UAICharacterAnimInstance::NativeInitializeAnimation()
{
	this->ownerPawn = Cast<AAICharacter>(TryGetPawnOwner());
	this->RandomDeathAnimationIndex = 0;
}

void UAICharacterAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	if (ownerPawn)
	{
		this->MovementStatus = this->ownerPawn->MovementStatus;
		this->IsAiming = this->ownerPawn->IsAiming;
		this->IsFiring = this->ownerPawn->IsFiring;
		this->IsAlerted = this->ownerPawn->IsAlerted;
		this->MovementDirection = this->ownerPawn->MovementDirection;
		this->WeaponType = this->ownerPawn->WeaponType;
		this->IsDead = this->ownerPawn->IsDead;
	}
}

void UAICharacterAnimInstance::UpdatePosition()
{
	if (ownerPawn)
	{
		this->PositionStatus = this->ownerPawn->PositionStatus;
	}
}


