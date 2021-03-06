// Nadir Román Guerrero. All right reserverd

#include "TFG_1.h"
#include "CustomUIEngineComponents.h"

UMyWorldSettings * UCustomUIEngineComponents::INSTANCE;

FResolutionStruct UCustomUIEngineComponents::SwitchResolution(FResolutionStruct res, bool decrease)
{
	FResolutionStruct result;
	if (decrease)
	{
		switch (res.Width)
		{
		case 1920:
			result.Width = 1280;
			result.Height = 720;
			break;
		case 1280:
			result.Width = 720;
			result.Height = 405;
			break;
		default:
			result.Width = res.Width;
			result.Height = res.Height;
		}
	}
	else
	{
		switch (res.Width)
		{
		case 720:
			result.Width = 1280;
			result.Height = 720;
			break;
		case 1280:
			result.Width = 1920;
			result.Height = 1080;
			break;
		default:
			result.Width = res.Width;
			result.Height = res.Height;
		}
	}

	return result;
}

void UCustomUIEngineComponents::UpdateInputSettings()
{
	UMyWorldSettings * singletonSettings = Cast<UMyWorldSettings>(GEngine->GameSingleton);
	UInputSettings *inputSettings = GetMutableDefault<UInputSettings>();

	inputSettings->ActionMappings.Empty();
	inputSettings->AxisMappings.Empty();

	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), singletonSettings->Forward, 1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), singletonSettings->Backward, -1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), singletonSettings->Right, 1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), singletonSettings->Left, -1.0f));

	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("Turn"), EKeys::MouseX, 1.0f * singletonSettings->MouseSensitivity));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("TurnRate"), EKeys::Left, -1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("TurnRate"), EKeys::Right, 1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("LookUp"), EKeys::MouseY, -1.0f * singletonSettings->MouseSensitivity));

	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Jump"), singletonSettings->Jump, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("AimDownSight"), singletonSettings->AimDownTheSight, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("ThrowGrenade"), singletonSettings->ThrowGrenade, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Action"), singletonSettings->Action, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("SwitchWeapon"), singletonSettings->SwitchWeapon, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Run"), singletonSettings->ToggleRun, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Reload"), singletonSettings->Reload, false, false, false, false));
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Fire"), singletonSettings->Fire, false, false, false, false));

	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Action"), singletonSettings->Action, false, false, false, false));

	inputSettings->SaveKeyMappings();
}

UMyWorldSettings * UCustomUIEngineComponents::GetWorldSettings(bool &IsValid)
{
	IsValid = false;

	UMyWorldSettings * Settings = Cast<UMyWorldSettings>(GEngine->GameSingleton);

	if (!Settings) return NULL;

	if (!Settings->IsValidLowLevel()) return NULL;

	IsValid = true;
	return Settings;
}

UObject * UCustomUIEngineComponents::CreateObjectFromClass(UObject * outer, UClass * uClass)
{
	return NewObject<UObject>(outer, uClass);
}