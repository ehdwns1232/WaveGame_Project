#include "WavePlayerController.h"
#include "EnhancedInputSubsystems.h"

AWavePlayerController::AWavePlayerController()
	: DefaultIMC(nullptr)
	, MoveAction(nullptr)
	, LookAction(nullptr)
	, JumpAction(nullptr)
	, SprintAction(nullptr)
{
}

void AWavePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultIMC, 0);
		}
	}
}