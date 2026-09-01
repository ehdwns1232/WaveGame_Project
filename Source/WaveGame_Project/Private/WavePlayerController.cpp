#include "WavePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "WaveGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenuWidget.h"
#include "UI/HUDWidget.h"
#include "Kismet/GameplayStatics.h"

AWavePlayerController::AWavePlayerController()
	: DefaultIMC(nullptr)
	, MoveAction(nullptr)
	, LookAction(nullptr)
	, JumpAction(nullptr)
	, SprintAction(nullptr)
	, PickupAction(nullptr)
	, HUDWidgetClass(nullptr)
	, HUDWidgetInstance(nullptr)
	, MainMenuWidgetClass(nullptr)
	, MainMenuWidgetInstance(nullptr)
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

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains(TEXT("MenuLevel"), ESearchCase::IgnoreCase))
	{
		ShowMainMenu(false, false);
	}
}

UHUDWidget* AWavePlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

UMainMenuWidget* AWavePlayerController::GetMainMenuWidget() const
{
	return MainMenuWidgetInstance;
}

void AWavePlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void AWavePlayerController::ShowMainMenu(bool bIsRestart, bool bIsClear)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetClass = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetClass = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
		if (!MainMenuWidgetInstance) return;
		
		MainMenuWidgetInstance->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		MainMenuWidgetInstance->UpdateMenuState(bIsRestart, bIsClear);
	}
}

void AWavePlayerController::StartGame()
{
	if (UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetWorld()->GetGameInstance()))
	{
		WaveGameInstance->SetCurrentLevelIndex(0);
		WaveGameInstance->SetTotalScore(0);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}

