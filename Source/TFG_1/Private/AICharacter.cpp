// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "AICharacter.h"
#include "AICharacterAnimInstance.h"

void UpdateAICharacterPosition(AAICharacter * AI, ECharacterPosition NewPosition)
{
	AI->PositionStatus = NewPosition;
}


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementStatus = ECharacterMovementStatus::VE_Idle;
	PositionStatus = ECharacterPosition::VE_StandingUp;
	MovementDirection = EMovementType::VE_Forward;

	LastTimeWeaponFired = 0.0f;

	HP = 100;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterWeapon"));
	Weapon->AttachParent = GetMesh();
	//Weapon->AttachTo(GetMesh(), CharacterWeaponAttachSocket);

	Magazine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterWeaponMagazine"));
	Magazine->AttachParent = Weapon;
	//Magazine->AttachTo(Weapon, MagazineSocket);

}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UCharPositionAnimNotify * StanceAnimNotifier = NewObject<UCharPositionAnimNotify>(UCharPositionAnimNotify::StaticClass());
	StanceAnimNotifier->Owner = this;

	TArray<UAnimMontage *> ChangeStanceAnimations;
	ChangeStanceAnimations.Add(AnimStandToCrouch);
	ChangeStanceAnimations.Add(AnimStandToCrouchAim);
	ChangeStanceAnimations.Add(AnimStandToProne);
	ChangeStanceAnimations.Add(AnimCrouchToStand);
	ChangeStanceAnimations.Add(AnimCrouchToStandAim);
	ChangeStanceAnimations.Add(AnimProneToStand);

	for (auto& Anim : ChangeStanceAnimations)
	{
		if (Anim != NULL)
		{
			for (auto Start = begin(Anim->Notifies); Start != end(Anim->Notifies); Start++)
			{
				(*Start).Notify = StanceAnimNotifier;
			}
		}
	}
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	//Weapon->AttachParent = GetMesh();

	Magazine->AttachTo(Weapon, MagazineSocket);

	Weapon->AttachTo(GetMesh(), CharacterWeaponAttachSocket);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	UpdateMovementDirection();
}

void AAICharacter::UpdateMovementDirection()
{
	FVector forward = GetActorForwardVector();
	FVector right = GetActorRightVector();
	FVector velocity = GetVelocity();
	velocity.Normalize();

	float forwardResult = FVector::DotProduct(forward, velocity);
	float rightResult = FVector::DotProduct(right, velocity);

	float forwardABS = FMath::Abs(forwardResult);
	float rightABS = FMath::Abs(rightResult);

	if (forwardABS >= rightABS)
	{
		if (forwardResult > 0.0f)
		{
			MovementDirection = EMovementType::VE_Forward;
		}
		else
		{
			MovementDirection = EMovementType::VE_Backward;
		}
	}
	else
	{
		if (rightResult > 0.0f)
		{
			MovementDirection = EMovementType::VE_Right;
		}
		else
		{
			MovementDirection = EMovementType::VE_Left;
		}
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AAICharacter::OnProjectileHit(FName BoneName)
{
	if (!IsDead)
	{
		if (BoneName.Compare(FName(TEXT("head"))) == 0) // Hitted on head bone
		{
			HP = HP - 60;
		}
		else
		{
			HP = HP - 25;
		}

		if (!IsInmortal && HP <= 0)
		{
			OnDeath();
		}
		else if (HitAnimations.Num() > 0)
		{
			int32 randomIndex = FMath::RandRange(0, HitAnimations.Num() - 1);
			CurrentHit = HitAnimations[randomIndex];

			this->PlayAnimMontage(CurrentHit, 1.0f, NAME_None);
		}
	}
}

void AAICharacter::DoFire()
{
	if (IsDead)
	{
		return;
	}

	if (!Weapon->DoesSocketExist(BurstSocket))
	{
		return;
	}

	IsFiring = true;

	UAnimMontage * AnimationToPlay = AnimationFire;

	if (PositionStatus == ECharacterPosition::VE_Proning)
	{
		AnimationToPlay = AnimationFireProning;
	}
	else if (IsAiming)
	{
		AnimationToPlay = AnimationFireAiming;
	}

	if (PositionStatus != ECharacterPosition::VE_Crouching)
	{
		this->PlayAnimMontage(AnimationToPlay, 1.0f, NAME_None);
	}

	if (ProjectileClass != NULL)
	{
		FVector MuzzleLoc = Weapon->GetSocketLocation(BurstSocket);
		float CurTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		FRotator ActorRotation = this->GetActorRotation();

		if (CurTime - LastTimeWeaponFired < 0.7f)
		{
			ActorRotation.Add(FMath::FRandRange(-2.0f, 2.0f), FMath::FRandRange(-3.0f, 3.0f), 0.0f);
		}

		FActorSpawnParameters spawnInfo;
		spawnInfo.Instigator = this;
		GetWorld()->SpawnActor<ATFG_1Projectile>(ProjectileClass, MuzzleLoc, ActorRotation, spawnInfo);

		LastTimeWeaponFired = CurTime;

		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}

	if (MuzzlePSC == NULL && MuzzlePS)
	{
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzlePS, Weapon, BurstSocket);
	}
}

void AAICharacter::StopFire()
{
	if (MuzzlePSC != NULL)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = NULL;
	}

	IsFiring = false;
}

void AAICharacter::OnDeath()
{
	IsDead = true;

	if (CurrentHit != NULL)
	{
		this->StopAnimMontage(CurrentHit);
	}

	if (IsFiring)
	{
		StopFire();
	}
}

bool AAICharacter::tryAddActorToKnownlist(AGameCharacter * enemy)
{
	if (enemy != NULL && enemy->TeamId != this->TeamId)
	{
		if (!enemy->IsDead)
		{
			bool previouslyInList = KnownList.Contains(enemy->ID);

			if (!previouslyInList)
			{
				UKnownEnemy * newEnemy = NewObject<UKnownEnemy>();
				newEnemy->SetCharacter(enemy);
				newEnemy->LastKnownLocation = enemy->GetActorLocation();
				KnownList.Add(enemy->ID, newEnemy);
				InformAllies();
				return true;
			}
			else
			{
				UKnownEnemy * enemyRef = KnownList[enemy->ID];
				if (enemyRef != NULL)
				{
					enemyRef->LastKnownLocation = enemy->GetActorLocation();
				}
			}

			return false;
		}

		return false;
	}

	return false;
}

void AAICharacter::TryRemoveActorFromKnownlist(AGameCharacter * character)
{
	if (character != NULL && character->TeamId != this->TeamId)
	{
		if (character->IsDead)
		{
			KnownList.Remove(character->ID);
		}
	}
}

TArray<UKnownEnemy *> AAICharacter::GetKnownActors()
{
	TArray<UKnownEnemy *> values;
	KnownList.GenerateValueArray(values);

	return values;
}

void AAICharacter::InformAllies()
{
	for (int32 i = 0; i < KnownAllies.Num(); i++)
	{
		AAICharacter * ally = KnownAllies[i];

		if (ally != NULL && !ally->IsDead)
		{
			for (auto It = KnownList.CreateIterator(); It; ++It)
			{
				UKnownEnemy * enemy = It.Value();
				if (enemy->Character != NULL && !enemy->Character->IsDead)
				{
					UKnownEnemy * newEntry = NewObject<UKnownEnemy>();
					newEntry->Character = enemy->Character;
					newEntry->LastKnownLocation = enemy->LastKnownLocation;
					ally->KnownList.Add(It.Key(), newEntry);
				}
			}
		}
	}
}

void AAICharacter::CleanKnownList()
{
	TArray<UKnownEnemy *> known = GetKnownActors();
	UKnownEnemy * enemy;
	int32 i = 0;
	while (i < known.Num())
	{
		enemy = known[i];
		i++;

		if (enemy->Character == NULL || enemy->Character->IsDead)
		{
			KnownList.Remove(enemy->Character->ID);
		}
	}
}

void AAICharacter::ForceDeleteKnown(FString CharID)
{
	KnownList.Remove(CharID);
}

int32 AAICharacter::SetPosition(ECharacterPosition NewPos)
{
	int32 AnimationLenght = 0;
	if (PositionStatus != NewPos)
	{
		UAnimMontage * animationToPlay = NULL;
		switch (PositionStatus)
		{
			case ECharacterPosition::VE_StandingUp:
			{
				switch (NewPos)
				{
					case ECharacterPosition::VE_Crouching:
					{
						if (IsAiming)
							animationToPlay = AnimStandToCrouchAim;
						else
							animationToPlay = AnimStandToCrouch;

						AnimationLenght = 26;
						break;
					}
					case ECharacterPosition::VE_Proning:
						animationToPlay = AnimStandToProne;
						AnimationLenght = 50;
						break;
				}

				Crouch();

				break;
			}
			case ECharacterPosition::VE_Crouching:
			{
				if (NewPos == ECharacterPosition::VE_StandingUp)
				{
					if (IsAiming)
						animationToPlay = AnimCrouchToStandAim;
					else
						animationToPlay = AnimCrouchToStand;

					AnimationLenght = 19;

					UnCrouch();
				}
				break;
			}
			case ECharacterPosition::VE_Proning:
			{
				if (NewPos == ECharacterPosition::VE_StandingUp)
				{
					animationToPlay = AnimProneToStand;
					AnimationLenght = 44;
					UnCrouch();
				}
				break;
			}

			if (animationToPlay != NULL)
			{
				this->PlayAnimMontage(animationToPlay, 1.0f, NAME_None);
			}
		}

		PositionStatus = NewPos;

	}

	return AnimationLenght;
}

// ==========================================================================================================================

void UCharPositionAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (Owner)
	{
		Cast<UAICharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance())->UpdatePosition();
	}
}