// Nadir Román Guerrero. All right reserverd

#pragma once

#include "GameFramework/Actor.h"
#include "../TFG_1Projectile.h"
#include "AttachmentComponent.h"
#include "AbstractWeapon.generated.h"

class AAbstractWeapon;

typedef void (__cdecl *StateHandler)(AAbstractWeapon &weapon, uint32 Action);

UCLASS()
class TFG_1_API AAbstractWeapon : public AActor
{
	GENERATED_BODY()
public:
	static const uint32 ACTION_FIRE;
	static const uint32 ACTION_RELOAD;

	static const uint32 STATE_OK;
	static const uint32 STATE_NEED_RELOAD;
	static const uint32 STATE_EMPTY;

	TArray<StateHandler> StateHandlers;
	int32 WeaponState;

	bool InfiniteAmmo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FireStats")
	int32 MagazineSize;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FireStats")
	int32 MaxAmmo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FireStats")
	int32 CurrentMagazineRounds;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FireStats")
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FireStats")
	float FireCadency;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FireStats")
	float RecoilEffectRate;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent * Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	FVector GunOffset;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent * MagMeshComponent;

	class ATFG_1Character * Owner;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh)
	FName MagazinePosition;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Attachments)
	TArray<TSubclassOf<UAttachmentComponent>> AttachmentList;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName MuzzleAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleFX;

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Info)
	UTexture * Thumbnail;

	// =====================================================================
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sounds")
	class USoundBase * FireSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sounds")
	class USoundBase * EmptySound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sounds")
	class USoundBase * ReloadSound;

	 // =====================================================================
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimSequence * IdleAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimSequence * WalkAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimMontage * ReloadAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimMontage * FireAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimSequence * AimAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimMontage * AimFireAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimSequence * RunAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations")
	class UAnimMontage * EquipAnimation;
	
	class UAnimMontage * CurrentFireAnimation;

	// =====================================================================
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ATFG_1Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<UCameraShake> FireCameraShake;

	// =====================================================================
	// =====================================================================
	// =====================================================================

	AAbstractWeapon();

	UFUNCTION()
	virtual void BeginPlay() override;

	virtual void Tick( float DeltaSeconds ) override;

	// Owner player management
	void SetOwner(ATFG_1Character * OwnerPtr);
	ATFG_1Character * GetOwner();

	// Attachment management
	void AttachToOwner();
	void DetachFromOwner();

	void OnEquip();
	void OnUnEquip();

	// Weapon Handle management
	void UpdateWeaponState();
	void HandleWeaponState(uint32 DesiredAction);

	void UpdateHUD();

	bool CanFireWeapon();
	void FireWeapon();
	void StopFireWeapon();
	bool CanReload();
	void StartReload();
	void Reload();
	void CancelReload();

	void useAttachment();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitializeComponents() override;

};
