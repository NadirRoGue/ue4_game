// Nadir Román Guerrero. All right reserverd

#pragma once

#include "GameFramework/Character.h"
#include "AbstractWeapon.h"
#include "KnownEnemy.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AICharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	VE_Forward UMETA(DisplayName = "Forward"),
	VE_Backward UMETA(DisplayName = "Backward"),
	VE_Left UMETA(DisplayName = "Left"),
	VE_Right UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	VE_Rifle UMETA(DisplayName = "Rifle"),
	VE_Shotgun UMETA(DisplayName = "Shotgun"),
	VE_Gun UMETA(DisplayName = "Gun")
};

UENUM(BlueprintType)
enum class ECharacterPosition : uint8
{
	VE_StandingUp UMETA(DisplayName = "Standing Up"),
	VE_Crouching UMETA(DisplayName = "Crouching"),
	VE_Proning UMETA(DisplayName = "Proning")
};

UENUM(BlueprintType)
enum class ECharacterMovementStatus : uint8
{
	VE_Idle UMETA(DisplayName = "Idle"),
	VE_Walking UMETA(DisplayName = "Walking"),
	VE_Running UMETA(DisplayName = "Running")
};

class UCharPositionAnimNotify;

UCLASS()
class TFG_1_API AAICharacter : public AGameCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	class USkeletalMeshComponent * Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	class USkeletalMeshComponent * Magazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	FName MagazineSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	FName BurstSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	class UParticleSystem * MuzzlePS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	FName CharacterWeaponAttachSocket;

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	UPROPERTY(BlueprintReadWrite)
	ECharacterMovementStatus MovementStatus;

	UPROPERTY(BlueprintReadWrite)
	ECharacterPosition PositionStatus;

	UPROPERTY(EditDefaultsOnly)
	bool IsInmortal;

	UPROPERTY(BlueprintReadWrite)
	bool IsFiring;

	UPROPERTY(BlueprintReadWrite)
	bool IsAiming;

	UPROPERTY(BlueprintReadWrite)
	bool IsAlerted;

	UPROPERTY(BlueprintReadWrite)
	EMovementType MovementDirection;

	UPROPERTY(BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage *> HitAnimations;

	UAnimMontage * CurrentHit;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage * AnimationFireAiming;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage * AnimationFire;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage * AnimationFireProning;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ATFG_1Projectile> ProjectileClass;

	float LastTimeWeaponFired;

	TMap<FString, UKnownEnemy *> KnownList;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TArray<AAICharacter *> KnownAllies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement settings")
	float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Change stance animations")
	UAnimMontage * AnimStandToCrouch;

	UPROPERTY(EditDefaultsOnly, Category = "Change stance animations")
	UAnimMontage * AnimStandToCrouchAim;

	UPROPERTY(EditDefaultsOnly, Category = "Change stance animations")
	UAnimMontage * AnimStandToProne;

	UPROPERTY(EditDefaultsOnly, Category = "Change stance animations")
	UAnimMontage * AnimCrouchToStand;

	UPROPERTY(EditDefaultsOnly, Category = "Change stance animations")
	UAnimMontage * AnimCrouchToStandAim;

	UPROPERTY(EditDefaultsOnly, Category = "Change stance animations")
	UAnimMontage * AnimProneToStand;

	UCharPositionAnimNotify * StanceAnimNotifier;

	// Sets default values for this character's properties
	AAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	UFUNCTION(BlueprintCallable, Category = "AI Responses")
	void OnProjectileHit(FName BoneName);

	UFUNCTION(BlueprintCallable, Category = "AI Actions")
	void DoFire();

	UFUNCTION(BlueprintCallable, Category = "AI Actions")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "AI Responses")
	void OnDeath();

	UFUNCTION(BlueprintCallable, Category = "AI Knownlist")
	bool tryAddActorToKnownlist(AGameCharacter * character);

	UFUNCTION(BlueprintCallable, Category = "AI Knownlist")
	void TryRemoveActorFromKnownlist(AGameCharacter * character);

	UFUNCTION(BlueprintCallable, Category = "AI Knownlist")
	TArray<UKnownEnemy *> GetKnownActors();

	UFUNCTION(BlueprintCallable, Category = "AI Knownlist")
	void InformAllies();

	UFUNCTION(BlueprintCallable, Category = "AI Knownlist")
	void CleanKnownList();

	UFUNCTION(BlueprintCallable, Category = "AI Knownlist")
	void ForceDeleteKnown(FString CharID);

	UFUNCTION(BlueprintCallable, Category = "Movement management")
	int32 SetPosition(ECharacterPosition NewPos);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sounds")
	class USoundBase * FireSound;

	virtual void PostInitializeComponents() override;

private:

	void UpdateMovementDirection();
};

UCLASS()
class TFG_1_API UCharPositionAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	AAICharacter * Owner;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};