// Nadir Román Guerrero. All right reserverd

#pragma once

#include "AbstractWeapon.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AbstractWeaponAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class TFG_1_API UAbstractWeaponAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	AAbstractWeapon * Weapon;
};

UCLASS()
class TFG_1_API UMagazinePickAnimNotify: public UAbstractWeaponAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class TFG_1_API UMagazineReleaseAnimNotify : public UAbstractWeaponAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};


UCLASS()
class TFG_1_API UReloadComplete : public UAbstractWeaponAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
