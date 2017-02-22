// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "TFG_1HUD.generated.h"

UCLASS()
class ATFG_1HUD : public AHUD
{
	GENERATED_BODY()

public:
	ATFG_1HUD();

	UPROPERTY(EditDefaultsOnly, Category = "HUD Features")
	bool DrawCrossHair;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

