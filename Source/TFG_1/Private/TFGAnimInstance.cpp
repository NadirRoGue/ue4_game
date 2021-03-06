// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "TFGAnimInstance.h"

void UTFGAnimInstance::NativeInitializeAnimation()
{
	this->ownerPawn = Cast<ATFG_1Character>(TryGetPawnOwner());
}

void UTFGAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	if (this->ownerPawn)
	{
		this->IsRunning = this->ownerPawn->IsRunning;
		this->IsWalking = this->ownerPawn->IsWalking;
		this->IsAiming = this->ownerPawn->IsAiming;
		this->IsFiring = this->ownerPawn->IsFiring;
		this->IsUnarmed = this->ownerPawn->IsUnarmed;
	}
}




