// Nadir Román Guerrero. All Rights Reserved.

#include "TFG_1.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

const int32 ATFG_1Character::PRIMARY_WEAPON = 0;
const int32 ATFG_1Character::SECONDARY_WEAPON = 1;

const FName ATFG_1Character::ReloadNotifyName("NReloadComplete");
const FName ATFG_1Character::MagPickNotifyName("PickMagazine");
const FName ATFG_1Character::MagPutNotifyName("ReleaseMagazine");

//////////////////////////////////////////////////////////////////////////
// ATFG_1Character

ATFG_1Character::ATFG_1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CurrentHP = 100;
	LastImpactTime = 0;

	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->AttachTo(RootComponent);
	CameraBoom->AttachParent = GetCapsuleComponent();
	//CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->RelativeLocation = FVector(0, 0, 64.f);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bDoCollisionTest = false;*/

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();//GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	ReloadCallback = NewObject<UReloadComplete>(UReloadComplete::StaticClass());
	PickMagCallback = NewObject<UMagazinePickAnimNotify>(UMagazinePickAnimNotify::StaticClass());
	PutMagCallback = NewObject<UMagazineReleaseAnimNotify>(UMagazineReleaseAnimNotify::StaticClass());

	//Weapons.SetNumUninitialized(2);
	CurrentWeaponIndex = 0;
}

void ATFG_1Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATFG_1Character::OnFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATFG_1Character::OnFireEnd);
	InputComponent->BindAxis("MoveForward", this, &ATFG_1Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATFG_1Character::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATFG_1Character::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATFG_1Character::LookUpAtRate);
	InputComponent->BindAction("Run", IE_Pressed, this, &ATFG_1Character::OnToggleRun);
	InputComponent->BindAction("Run", IE_Released, this, &ATFG_1Character::UnToggleRun);
	InputComponent->BindAction("AimDownSight", IE_Pressed, this, &ATFG_1Character::StartAiming);
	InputComponent->BindAction("AimDownSight", IE_Released, this, &ATFG_1Character::EndAiming);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ATFG_1Character::Reload);
	InputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ATFG_1Character::ThrowGranade);
	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ATFG_1Character::SwitchWeapon);
	InputComponent->BindAction("Action", IE_Pressed, this, &ATFG_1Character::Action);
}

void ATFG_1Character::Action_Implementation()
{

}

void ATFG_1Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bool isValid = false;

	UMyWorldSettings * settings = UCustomUIEngineComponents::GetWorldSettings(isValid);

	if (!isValid)
	{
		return;
	}

	if (settings->WeaponClass == NULL)
	{
		Mesh1P->SetVisibility(false, true);
	}
	else
	{
		WeaponTemplates.Add(settings->WeaponClass);
		for (int32 i = 0; i < WeaponTemplates.Num(); i++)
		{
			if (WeaponTemplates[i])
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.bNoCollisionFail = true;
				AAbstractWeapon * NewWeap = GetWorld()->SpawnActor<AAbstractWeapon>(WeaponTemplates[i], SpawnInfo);
				Inventory.Add(NewWeap);
			}
		}
		
		CurrentWeaponIndex = 0;
		EquipWeapon();
	}
}

void ATFG_1Character::EquipWeapon()
{
	if (!Mesh1P->IsVisible())
	{
		Mesh1P->SetVisibility(true, true);
	}

	CurrentWeapon = Inventory[CurrentWeaponIndex];

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->OnEquip();
		CurrentWeapon->AttachToOwner();
		UpdateReloadCallBack();
	}
}

void ATFG_1Character::UnEquipWeapon()
{
	CurrentWeapon->OnUnEquip();
	CurrentWeapon->DetachFromOwner();
}

void ATFG_1Character::OnFire()
{
	if (IsUnarmed)
	{
		return;
	}

	if (CurrentWeapon)
	{
		if (!IsReloading || (IsReloading && CurrentWeapon->CanFireWeapon()))
		{
			CurrentWeapon->CancelReload();
			IsFiring = true;
			HandleFire();
		}
	}
}

void ATFG_1Character::OnFireEnd()
{
	if (IsFiring)
	{
		IsFiring = false;
		CurrentWeapon->StopFireWeapon();
		GetWorldTimerManager().ClearTimer(Firehandle);
	}
}

void ATFG_1Character::HandleFire()
{
	if (!IsFiring)
		return;

	float NextTimer = -1.0f;
	if (CurrentWeapon->CanFireWeapon())
		NextTimer = 1.0f / CurrentWeapon->FireCadency;
	else if (CurrentWeapon->CanReload())
		NextTimer = CurrentWeapon->ReloadAnimation->SequenceLength;

	if (NextTimer > -1.0f)
	{
		GetWorldTimerManager().SetTimer(Firehandle, this, &ATFG_1Character::HandleFire, NextTimer, false);
		CurrentWeapon->HandleWeaponState(AAbstractWeapon::ACTION_FIRE);
	}
	else
		OnFireEnd();
}

void ATFG_1Character::MoveForward(float Value)
{
	if (IsUnarmed)
	{
		return;
	}

	if (Value != 0.0f)
	{
		if (IsRunning && Value < 0.0f)
			UnToggleRun();

		if (!IsWalking)
			IsWalking = true;
		AddMovementInput(GetActorForwardVector(), Value);
	}
	else
	{
		if (IsRunning)
			UnToggleRun();

		IsWalking = false;
	}
}

void ATFG_1Character::MoveRight(float Value)
{
	if (IsUnarmed)
	{
		return;
	}

	if (Value != 0.0f)
	{
		UnToggleRun();
		IsWalking = true;
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATFG_1Character::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATFG_1Character::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATFG_1Character::OnToggleRun()
{
	if (IsUnarmed)
	{
		return;
	}

	if (!IsAiming && IsWalking && !IsRunning)
	{
		if (IsReloading && CurrentWeapon)
			CurrentWeapon->CancelReload();

		IsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed *= 2.5;
	}
}

void ATFG_1Character::UnToggleRun()
{
	if (IsRunning)
	{
		IsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed /= 2.5;
	}
}

void ATFG_1Character::StartAiming_Implementation()
{
	if (IsUnarmed)
	{
		return;
	}

	if (IsReloading && CurrentWeapon)
		CurrentWeapon->CancelReload();

	UnToggleRun();
	IsAiming = true;
	float CurrentFOV = FirstPersonCameraComponent->FieldOfView;
	FirstPersonCameraComponent->FieldOfView = 60.0f;
}

void ATFG_1Character::EndAiming_Implementation()
{
	IsAiming = false;
	float CurrentFOV = FirstPersonCameraComponent->FieldOfView;
	FirstPersonCameraComponent->FieldOfView = 90.0f;
}

void ATFG_1Character::Reload()
{
	if (IsUnarmed)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->HandleWeaponState(AAbstractWeapon::ACTION_RELOAD);
	}
}

void ATFG_1Character::SwitchWeapon()
{
	if (IsUnarmed)
	{
		return;
	}

	CurrentWeaponIndex = ((CurrentWeaponIndex + 1) % WeaponTemplates.Num());

	if (CurrentWeapon)
	{
		if (IsReloading)
			CurrentWeapon->CancelReload();

		CurrentWeapon->DetachFromOwner();
		CurrentWeapon->OnUnEquip();
	}

	EquipWeapon();
}

void ATFG_1Character::ThrowGranade()
{
	if (IsUnarmed)
	{
		return;
	}

	if (IsReloading && CurrentWeapon)
		CurrentWeapon->CancelReload();

}

void ATFG_1Character::PlayAnimation(UAnimMontage *Montage)
{
	UAnimInstance * AnimInstance = Mesh1P->GetAnimInstance();
	if (Montage && AnimInstance)
	{
		AnimInstance->Montage_Play(Montage, 1.f);
	}
}

void ATFG_1Character::StopAnimation(UAnimMontage *Montage)
{
	UAnimInstance * AnimInstance = Mesh1P->GetAnimInstance();
	if (Montage && AnimInstance)
	{
		AnimInstance->Montage_Stop(0.05f, Montage);
	}
}

void ATFG_1Character::PauseAnimation(UAnimMontage *Montage)
{
	UAnimInstance * AnimInstance = Mesh1P->GetAnimInstance();
	if (Montage && AnimInstance && AnimInstance->Montage_IsPlaying(Montage))
	{
		Mesh1P->GetAnimInstance()->Montage_Pause(Montage);
	}
}

void ATFG_1Character::UpdateReloadCallBack()
{
	if (CurrentWeapon && CurrentWeapon->ReloadAnimation)
	{
		TMap<FName, FAnimNotifyEvent*> MappedNotifies;
		for (auto Start = begin(CurrentWeapon->ReloadAnimation->Notifies); Start != end(CurrentWeapon->ReloadAnimation->Notifies); Start++)
		{
			FName NotifyName = (*Start).NotifyName;
			MappedNotifies.Add(NotifyName, &(*Start));
		}

		ReloadCallback->Weapon = CurrentWeapon;
		PickMagCallback->Weapon = CurrentWeapon;
		PutMagCallback->Weapon = CurrentWeapon;

		MappedNotifies[ReloadNotifyName]->Notify = ReloadCallback;
		MappedNotifies[MagPickNotifyName]->Notify = PickMagCallback;
		MappedNotifies[MagPutNotifyName]->Notify = PutMagCallback;
	}
}

int32 ATFG_1Character::GetWeaponMagazine()
{
	return (CurrentWeapon->CurrentMagazineRounds);
}

int32 ATFG_1Character::GetWeaponAmmo()
{
	return (CurrentWeapon->CurrentAmmo);
}