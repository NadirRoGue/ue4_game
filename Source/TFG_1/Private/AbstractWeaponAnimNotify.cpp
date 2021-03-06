// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "AbstractWeaponAnimNotify.h"


void UMagazinePickAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (Weapon && Weapon->MagMeshComponent)
	{
		Weapon->MagMeshComponent->AttachTo(Weapon->Owner->GetMesh1P(), FName("MagHolder"));
	}
}

void UMagazineReleaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (Weapon && Weapon->MagMeshComponent)
	{
		Weapon->MagMeshComponent->AttachTo(Weapon->Mesh, FName("MagPosition"));
	}
}

void UReloadComplete::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (Weapon)
	{
		Weapon->Reload();
		Weapon->UpdateWeaponState();
		Weapon->UpdateHUD();
	}
}
