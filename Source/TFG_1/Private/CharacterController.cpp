// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "CharacterController.h"


ACharacterController::ACharacterController(const FObjectInitializer & ObjectInitializer) :Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ACameraUpdater::StaticClass();
	bAutoManageActiveCameraTarget = false;
	bFindCameraComponentWhenViewTarget = true;
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	SetViewTarget(GetPawn());
}

