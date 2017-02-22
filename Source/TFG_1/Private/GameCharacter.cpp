// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "GameCharacter.h"


// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HP = 100.0f;
	IsDead = false;

}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ID = GetName();
}

// Called every frame
void AGameCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AGameCharacter::OnProjectileCollision_Implementation(FName HitBone)
{

}