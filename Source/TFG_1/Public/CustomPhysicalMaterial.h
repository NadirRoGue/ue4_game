// Nadir Román Guerrero. All right reserverd

#pragma once

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CustomPhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TFG_1_API UCustomPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HitParticleSystem")
	class UParticleSystem * OnHitParticleSystem;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HitSound")
	class USoundWave * OnHitSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "StepSound")
	class USoundWave * OnStepSound;
};
