#include "UI/DebuffWidget.h"
#include "Player/PlayerCharacter.h"
#include "Components/TextBlock.h"

void UDebuffWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn()))
	{
		PlayerCharacter->OnSlowStackChanged.AddDynamic(this, &UDebuffWidget::UpdateSlowDebuff);
		UpdateSlowDebuff(PlayerCharacter->GetSlowStack());
		PlayerCharacter->OnReverseStackChanged.AddDynamic(this, &UDebuffWidget::UpdateReverseDebuff);
		UpdateReverseDebuff(PlayerCharacter->GetReverseStack());
	}
	SlowStackText->SetVisibility(ESlateVisibility::Collapsed);
	ReverseStackText->SetVisibility(ESlateVisibility::Collapsed);
}

void UDebuffWidget::UpdateSlowDebuff(int32 SlowStack)
{
	if (!SlowStackText) return;

	if (SlowStack > 0)
	{
		SlowStackText->SetText(FText::FromString(FString::Printf(TEXT("Slow Debuff : %d"), SlowStack)));
		SlowStackText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SlowStackText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDebuffWidget::UpdateReverseDebuff(int32 ReverseStack)
{
	if (!ReverseStackText) return;

	if (ReverseStack > 0)
	{
		ReverseStackText->SetText(FText::FromString(FString::Printf(TEXT("Reverse Debuff : %d"), ReverseStack)));
		ReverseStackText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReverseStackText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
