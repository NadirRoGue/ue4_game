// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "KnownEnemy.h"

void UKnownEnemy::SetCharacter(AGameCharacter * enemy)
{
	if (enemy != NULL && !enemy->IsDead)
	{
		Character = enemy;
		Update();
	}
}

void UKnownEnemy::Update()
{
	if (Character != NULL && !Character->IsDead)
	{
		FVector loc = Character->GetActorLocation();
		LastKnownLocation.X = loc.X;
		LastKnownLocation.Y = loc.Y;
		LastKnownLocation.Z = loc.Z;
	}
}


