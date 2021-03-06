// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "CameraUpdater.h"

ACameraUpdater::ACameraUpdater(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	AimFOV = 60.f;
	NormalFOV = 90.f;
	bAlwaysApplyModifiers = true;
	bFindCameraComponentWhenViewTarget = true;
	//setFov
}

void ACameraUpdater::UpdateCamera(float DeltaTime)
{
	ATFG_1Character * Player = this->PCOwner ? Cast<ATFG_1Character>(this->PCOwner->GetPawn()) : NULL;
	
	if (Player)
	{
		if (Player->IsAiming && GetFOVAngle() != AimFOV)
		{
			//UnlockFOV();
			SetFOV(FMath::FInterpTo(GetFOVAngle(), AimFOV, DeltaTime, 20.f));
		}
		else if (!Player->IsAiming && GetFOVAngle() != NormalFOV)
		{
			//UnlockFOV();
			SetFOV(FMath::FInterpTo(GetFOVAngle(), NormalFOV, DeltaTime, 20.f));
		}
	}

	Super::UpdateCamera(DeltaTime);
}
