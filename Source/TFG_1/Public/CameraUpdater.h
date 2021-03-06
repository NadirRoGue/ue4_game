// Nadir Román Guerrero. All right reserverd

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "CameraUpdater.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API ACameraUpdater : public APlayerCameraManager
{
	GENERATED_UCLASS_BODY()
public:
	float NormalFOV;
	float AimFOV;

	virtual void UpdateCamera(float DeltaTime) override;
};
