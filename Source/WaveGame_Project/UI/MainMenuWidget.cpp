#include "UI/MainMenuWidget.h"
#include "WavePlayerController.h"
#include "WaveGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClick);
		StartButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnStartButtonHoverAnim);
		StartButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnStartButtonUnHoverAnim);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClick);
		ExitButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnExitButtonHoverAnim);
		ExitButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnExitButtonUnHoverAnim);
	}
	if (UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance()))
	{
		WaveGameInstance->OnTotalScoreChanged.AddDynamic(this, &UMainMenuWidget::UpdateTotalScore);
		UpdateTotalScore(WaveGameInstance->GetTotalScore());
	}
}

void UMainMenuWidget::UpdateTotalScore(int32 TotalScore)
{
	if (!TotalScoreText) return;

	TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score : %d"), TotalScore)));
}

void UMainMenuWidget::UpdateMenuState(bool bIsRestart, bool bIsClear)
{
	if (!StartButtonText) return;

	if (!bIsRestart)
	{
		StartButtonText->SetText(FText::FromString(FString::Printf(TEXT("START"))));
		GameOverText->SetVisibility(ESlateVisibility::Hidden);
		TotalScoreText->SetVisibility(ESlateVisibility::Hidden);
		bIsGameOver = false;
	}
	else
	{
		StartButtonText->SetText(FText::FromString(FString::Printf(TEXT("RESTART"))));
		if (bIsClear)
		{
			GameOverText->SetText(FText::FromString(FString::Printf(TEXT("GAME CLEAR"))));
			GameOverText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		}
		else
		{
			GameOverText->SetText(FText::FromString(FString::Printf(TEXT("GAME OVER"))));
			GameOverText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		}
		GameOverText->SetVisibility(ESlateVisibility::Visible);
		TotalScoreText->SetVisibility(ESlateVisibility::Visible);
		PlayGameOverAnim();
		bIsGameOver = true;
	}

}

void UMainMenuWidget::OnStartButtonClick()
{
	if (AWavePlayerController* PlayerController = Cast<AWavePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->StartGame();
		StopAnimation(GameOverAnim);
	}
}

void UMainMenuWidget::OnExitButtonClick()
{
	if (AWavePlayerController* PlayerController = Cast<AWavePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	}
}

void UMainMenuWidget::OnStartButtonHoverAnim()
{
	if (StartButtonHoverAnim)
	{
		PlayAnimation(StartButtonHoverAnim, 0.0f, 0);
	}
}

void UMainMenuWidget::OnExitButtonHoverAnim()
{
	if (ExitButtonHoverAnim)
	{
		PlayAnimation(ExitButtonHoverAnim, 0.0f, 0);
	}
}


void UMainMenuWidget::OnStartButtonUnHoverAnim()
{
	if (StartButtonHoverAnim)
	{
		StopAnimation(StartButtonHoverAnim);
	}
}

void UMainMenuWidget::OnExitButtonUnHoverAnim()
{
	if (ExitButtonHoverAnim)
	{
		StopAnimation(ExitButtonHoverAnim);
	}
}

void UMainMenuWidget::PlayGameOverAnim()
{
	if (GameOverAnim)
	{
		PlayAnimation(GameOverAnim, 0.0f, 0);
	}
}


