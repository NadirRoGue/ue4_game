// Nadir Román Guerrero. All right reserverd

#pragma once

#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API ACharacterController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
		virtual void BeginPlay() override;

};
