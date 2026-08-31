#include "UI/TimeWidget.h"
#include "WaveGameState.h"
#include "Components/TextBlock.h"

void UTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(UpdateRemainingTimeHandle, this, &UTimeWidget::UpdateRemainingTime, 0.1f, true);
		WaveGameState = Cast<AWaveGameState>(World->GetGameState());
	}
}

void UTimeWidget::UpdateRemainingTime()
{
	if (!TimeText) return;
	
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(WaveGameState->WaveTimerHandle);

	TimeText->SetText(FText::FromString(FString::Printf(TEXT("TIME : %.1f"), RemainingTime)));

	if (RemainingTime <= 10.0f)
	{
		if (!IsAnimationPlaying(TimeAnim))
		{
			PlayAnimation(TimeAnim);
		}
	}
	else
	{
		StopAnimation(TimeAnim);
	}
}
