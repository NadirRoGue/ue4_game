// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "AbstractWeapon.h"
#include "CustomUIEngineComponents.h"

void StateHandlerOK(AAbstractWeapon & Weapon, uint32 Action)
{
	if (Action == AAbstractWeapon::ACTION_FIRE)
	{
		if (Weapon.CanFireWeapon())
			Weapon.FireWeapon();
		else if (Weapon.CanReload())
			Weapon.StartReload();
	}
	else if (Action == AAbstractWeapon::ACTION_RELOAD && Weapon.CanReload())
	{
		Weapon.StartReload();
	}
}

void StateHandlerRELOAD(AAbstractWeapon & Weapon, uint32 Action)
{
	Weapon.StartReload();
}

void StateHandlerEMPTY(AAbstractWeapon & Weapon, uint32 Action)
{
	// Play empty sound
}

const uint32 AAbstractWeapon::ACTION_FIRE = 0;
const uint32 AAbstractWeapon::ACTION_RELOAD = 1;

const uint32 AAbstractWeapon::STATE_OK = 0;
const uint32 AAbstractWeapon::STATE_NEED_RELOAD = 1;
const uint32 AAbstractWeapon::STATE_EMPTY = 2;

// Sets default values
AAbstractWeapon::AAbstractWeapon()
{
 	PrimaryActorTick.bCanEverTick = false;
	InfiniteAmmo = false;
	WeaponState = STATE_OK;

	RootComponent = Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	Mesh->bCastDynamicShadow = false;

	MagMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagazineMeshComponent"));
	MagMeshComponent->bCastDynamicShadow = false;
	
	//this->RootComponent = Mesh;

	StateHandlers.SetNumUninitialized(3);
	StateHandlers[STATE_OK] = StateHandlerOK;
	StateHandlers[STATE_NEED_RELOAD] = StateHandlerRELOAD;
	StateHandlers[STATE_EMPTY] = StateHandlerEMPTY;
}

// Called when the game starts or when spawned
void AAbstractWeapon::BeginPlay()
{
	Super::BeginPlay();

	MagMeshComponent->AttachTo(Mesh, this->MagazinePosition);

	bool isValid = false;

	UMyWorldSettings * settings = UCustomUIEngineComponents::GetWorldSettings(isValid);

	if (!isValid)
	{
		return;
	}

	AttachmentList.Empty();
	AttachmentList.Append(settings->AttachmentClasses);

	TArray<UAttachmentComponent *> tempAttachmentList;
	for (int i = 0; i < AttachmentList.Num(); i++)
	{
		UAttachmentComponent * attachment = NewObject<UAttachmentComponent>(Mesh, AttachmentList[i]);
		bool add = true;
		for (int j = 0; j < tempAttachmentList.Num() && add; j++)
		{
			add = attachment->isCompatibleWith(tempAttachmentList[j]);
		}

		if (add && Mesh->DoesSocketExist(attachment->SocketToAttach))
		{
			attachment->RegisterComponent();
			attachment->AttachTo(Mesh, attachment->SocketToAttach);
			attachment->initialize();
			tempAttachmentList.Add(attachment);
		}
	}
}

// Called every frame
void AAbstractWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AAbstractWeapon::UpdateWeaponState()
{
	if (!InfiniteAmmo)
	{
		if (CurrentMagazineRounds < 1)
		{
			if (CurrentAmmo > 0)
				WeaponState = STATE_NEED_RELOAD;
			else
				WeaponState = STATE_EMPTY;
		}
		else
		{
			WeaponState = STATE_OK;
		}
	}
}

void AAbstractWeapon::HandleWeaponState(uint32 DesiredAction)
{
	StateHandler Handler = StateHandlers[WeaponState];

	Handler(*this, DesiredAction);

	UpdateWeaponState();
	UpdateHUD();
}

void AAbstractWeapon::UpdateHUD()
{

}

bool AAbstractWeapon::CanFireWeapon()
{
	return WeaponState == STATE_OK;
}

void AAbstractWeapon::FireWeapon()
{
	FVector MuzzleLoc = Mesh->GetSocketLocation(MuzzleAttachPoint);

	// Fire effect
	if (MuzzlePSC == NULL)
	{
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh, MuzzleAttachPoint);
	}

	// Fire projectile
	if (ProjectileClass != NULL)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Owner;
		FRotator SpawnRotation = Owner->GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = MuzzleLoc;/* +SpawnRotation.RotateVector(GunOffset);*/
		GetWorld()->SpawnActor<ATFG_1Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnInfo);
	}

	// Fire sound
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (Owner->IsAiming)
	{
		if (CurrentFireAnimation != AimFireAnimation)
			Owner->StopAnimation(CurrentFireAnimation);
		CurrentFireAnimation = AimFireAnimation;
	}
	else
	{
		if (CurrentFireAnimation != FireAnimation)
			Owner->StopAnimation(CurrentFireAnimation);
		CurrentFireAnimation = FireAnimation;
	}

	Owner->PlayAnimation(CurrentFireAnimation);

	if (FireCameraShake)
	{
		APlayerController * PController = Cast<APlayerController>(Owner->GetController());
		if(PController)
		{
			PController->ClientPlayCameraShake(FireCameraShake, 1.f);
		}
	}

	CurrentMagazineRounds--;
}

void AAbstractWeapon::StopFireWeapon()
{
	if (MuzzlePSC != NULL)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = NULL;
	}

	Owner->StopAnimation(CurrentFireAnimation);
}

bool AAbstractWeapon::CanReload()
{
	return CurrentMagazineRounds < MagazineSize && CurrentAmmo > 0;
}

void AAbstractWeapon::StartReload()
{
	Owner->IsReloading = true;
	StopFireWeapon();
	Owner->PlayAnimation(ReloadAnimation);
}

void AAbstractWeapon::Reload()
{
	if (Owner->IsReloading)
	{
		int32 AvailableSpace = MagazineSize - CurrentMagazineRounds;
		int32 ReloadAmount = AvailableSpace < CurrentAmmo ? AvailableSpace : CurrentAmmo;
		CurrentMagazineRounds += ReloadAmount;
		CurrentAmmo -= ReloadAmount;

		Owner->IsReloading = false;

		if (ReloadSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
		}
	}
}

void AAbstractWeapon::CancelReload()
{
	Owner->StopAnimation(ReloadAnimation);
	Owner->IsReloading = false;
	MagMeshComponent->AttachTo(Mesh, this->MagazinePosition);
	GetWorldTimerManager().ClearTimer(Owner->ReloadHandle);
}

void AAbstractWeapon::OnConstruction(const FTransform& Transform)
{
}

void AAbstractWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MagMeshComponent->AttachTo(Mesh, this->MagazinePosition);
}

void AAbstractWeapon::SetOwner(ATFG_1Character * OwnerPtr)
{
	Owner = OwnerPtr;
}

ATFG_1Character * AAbstractWeapon::GetOwner()
{
	return Owner;
}

void AAbstractWeapon::AttachToOwner()
{
	if (Owner)
	{
		Mesh->SetVisibility(true, true);
		Mesh->AttachTo(Owner->GetMesh1P(), FName("WeaponHolder"));
		Owner->PlayAnimation(EquipAnimation);
	}
}

void AAbstractWeapon::DetachFromOwner()
{
	if (Owner)
	{
		Mesh->DetachFromParent();
		Mesh->SetVisibility(false, true);
	}
}

void AAbstractWeapon::OnEquip()
{
	UTFGAnimInstance * AnimInstance = Cast<UTFGAnimInstance>(Owner->GetMesh1P()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->IdleAnimation = this->IdleAnimation;
		AnimInstance->WalkAnimation = this->WalkAnimation;
		AnimInstance->RunAnimation = this->RunAnimation;
		AnimInstance->AimAnimation = this->AimAnimation;
	}
}

void AAbstractWeapon::OnUnEquip()
{
	
}

void AAbstractWeapon::useAttachment()
{
	
}