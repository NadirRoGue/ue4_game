// Nadir Román Guerrero. All right reserverd

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "AttachmentComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TFG_1_API UAttachmentComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UAttachmentComponent();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh)
	FName SocketToAttach;

	UFUNCTION(BlueprintImplementableEvent)
	virtual void initialize();

	UFUNCTION(BlueprintImplementableEvent)
	virtual bool isCompatibleWith(UAttachmentComponent * attachment);

	UFUNCTION(BlueprintImplementableEvent)
	virtual void useAttachment();

	UFUNCTION(BlueprintImplementableEvent)
	virtual void disableAttachment();
};
