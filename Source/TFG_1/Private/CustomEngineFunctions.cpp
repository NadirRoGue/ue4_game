// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "CustomEngineFunctions.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"

UActorComponent * UCustomEngineFunctions::AddActorComponent(AActor * owner, UClass* ActorComponentClass)
{
	UClass * baseClass = FindObject<UClass>(ANY_PACKAGE, TEXT("ActorComponent"));
	if (ActorComponentClass->IsChildOf(baseClass))
	{
		UActorComponent* NewComp = NewObject<UActorComponent>(owner, ActorComponentClass);
		if (!NewComp)
		{
			return NULL;
		}
		
		NewComp->RegisterComponent();  

		return NewComp;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create an instance of the ActorComponent"));
		return NULL;
	}
}

FVector UCustomEngineFunctions::CalcWalkablePoint(float MinDistance, float MaxDistance, AActor * Actor, FVector targetPos)
{
	FVector actorPos = Actor->GetActorLocation();

	MinDistance = MinDistance > MaxDistance ? MaxDistance : MinDistance;
	float finalDistance = FMath::RandRange(MinDistance, MaxDistance);

	FVector posVector = targetPos - actorPos;
	float posVectorLen = posVector.Size();

	float X, Y, Z;

	if (finalDistance < posVectorLen)
	{
		float alpha = 1 - (finalDistance / posVectorLen);
		
		float deltaX = posVector.X * alpha;
		float deltaY = posVector.Y * alpha;
		float deltaZ = (posVector.Z - actorPos.Z) * alpha;

		X = actorPos.X + deltaX;
		Y = actorPos.Y + deltaY;
		Z = actorPos.Z + deltaZ;
	}
	// We want to get a point more distant to target
	else
	{
		float alpha = finalDistance / posVectorLen;

		float deltaX = posVector.X * alpha - posVector.X;
		float deltaY = posVector.Y * alpha - posVector.Y;
		
		X = actorPos.X - deltaX;
		Y = actorPos.Y - deltaY;
		Z = actorPos.Z;
	}

	FVector Test(X, Y, Z);

	UNavigationPath* NavPath = UNavigationSystem::FindPathToLocationSynchronously(Actor->GetWorld(), actorPos, Test, NULL);

	if (!NavPath)
	{
		return actorPos;
	}

	if (NavPath->IsPartial())
	{
		return NavPath->GetPath()->GetEndLocation();
	}

	return Test;

}

/*FVector UCustomEngineFunctions::CalcWalkablePointSideways(float MinDistance, float MaxDistance, AActor * Actor, bool RightMovement)
{
	FVector RightVector = Actor->GetActorRightVector();
	if (!RightMovement)
	{
		RightVector *= -1;
	}

	MinDistance = MinDistance > MaxDistance ? MaxDistance : MinDistance;
	float finalDistance = FMath::RandRange(MinDistance, MaxDistance);

	RightVector *= finalDistance;

	FVector Test(Actor->GetActorLocation() + RightVector);

	UNavigationPath* NavPath = UNavigationSystem::FindPathToLocationSynchronously(Actor->GetWorld(), Actor->GetActorLocation(), Test, NULL);

	if (!NavPath)
	{
		return Actor->GetActorLocation();
	}

	if (NavPath->IsPartial())
	{
		return NavPath->GetPath()->GetDestinationLocation();
	}

	return Test;
}*/