#include "WavePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "WaveGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "WaveGameState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

AWavePlayerController::AWavePlayerController()
	: DefaultIMC(nullptr)
	, MoveAction(nullptr)
	, LookAction(nullptr)
	, JumpAction(nullptr)
	, SprintAction(nullptr)
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
		ShowMainMenu(false);
	}
}

UUserWidget* AWavePlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

UUserWidget* AWavePlayerController::GetMainMenuWidget() const
{
	return MainMenuWidgetInstance;
}

void AWavePlayerController::ShowGameHUD()
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

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}

	if (AWaveGameState* WaveGameState = Cast<AWaveGameState>(GetWorld()->GetGameState()))
	{
		WaveGameState->UpdateHUD();
	}
}

void AWavePlayerController::ShowMainMenu(bool bIsRestart)
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
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (!MainMenuWidgetInstance) return;
		
		MainMenuWidgetInstance->AddToViewport();

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		
		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("RESTART")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("START")));
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText"))))
			{
				if (UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetWorld()->GetGameInstance()))
				{
					int32 TotalScore = WaveGameInstance->GetTotalScore();
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("TotalScore : %d"), TotalScore)));
				}
			}
		}
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
}
