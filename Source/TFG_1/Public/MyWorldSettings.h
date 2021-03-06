// Nadir Román Guerrero. All right reserverd

#pragma once

#include "GameFramework/WorldSettings.h"
#include "AbstractWeapon.h"
#include "AttachmentComponent.h"
#include "MyWorldSettings.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UMyWorldSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings" )
	FKey Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Backward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey AimDownTheSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey SwitchWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey ToggleRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey ThrowGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	FKey Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	float MouseSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Weapon")
	TSubclassOf<AAbstractWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Weapon")
	TArray< TSubclassOf<UAttachmentComponent> > AttachmentClasses;
};
