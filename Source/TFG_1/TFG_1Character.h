// Nadir Román Guerrero. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "AbstractWeapon.h"
#include "GameCharacter.h"
#include "AbstractWeaponAnimNotify.h"
#include "TFG_1Character.generated.h"

class UInputComponent;
class AAbstractWeapon;

UCLASS()/*config=Game*/
class ATFG_1Character : public AGameCharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ATFG_1Character();

	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//virtual void OnConstruction(const FTransform& Transform) override;

	// =====================================================================
	// WEAPONS
	// =====================================================================
	static const int32 PRIMARY_WEAPON ;
	static const int32 SECONDARY_WEAPON;

	static const FName ReloadNotifyName;
	static const FName MagPickNotifyName;
	static const FName MagPutNotifyName;

	int32 CurrentWeaponIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AAbstractWeapon * CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<TSubclassOf<AAbstractWeapon>> WeaponTemplates;

	TArray<AAbstractWeapon *> Inventory;

	UAbstractWeaponAnimNotify *ReloadCallback;
	UAbstractWeaponAnimNotify *PickMagCallback;
	UAbstractWeaponAnimNotify *PutMagCallback;

	// =====================================================================
	// PLAYER PROPERTIES
	// =====================================================================


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsUnarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float LastImpactTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimSequence * IdleAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimSequence * WalkAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage * FireAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimSequence * AimAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage * AimFireAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage * RunAnimation;

	FTimerHandle ReloadHandle;
	FTimerHandle Firehandle;

public:
	
	void EquipWeapon();
	void UnEquipWeapon();

	void OnFire();
	void OnFireEnd();
	void HandleFire();
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void OnToggleRun();
	void UnToggleRun();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void StartAiming();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void EndAiming();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay")
	void Action();

	void Reload();
	void SwitchWeapon();
	void ThrowGranade();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetWeaponMagazine();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetWeaponAmmo();

	void UpdateReloadCallBack();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void PostInitializeComponents() override;

public:
	void PlayAnimation(UAnimMontage *Montage);
	void StopAnimation(UAnimMontage *Montage);
	void PauseAnimation(UAnimMontage *Montage);
};

