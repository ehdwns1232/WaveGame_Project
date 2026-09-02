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
	SetVisibility(ESlateVisibility::Collapsed);
	SlowStackText->SetVisibility(ESlateVisibility::Collapsed);
	ReverseStackText->SetVisibility(ESlateVisibility::Collapsed);
}

void UDebuffWidget::UpdateSlowDebuff(int32 SlowStack)
{
	if (!SlowStackText) return;

	if (SlowStack > 0)
	{
		SlowStackText->SetText(FText::FromString(FString::Printf(TEXT("Slow Debuff : %d"), SlowStack)));
		SetVisibility(ESlateVisibility::Visible);
		SlowStackText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		SlowStackText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDebuffWidget::UpdateReverseDebuff(int32 ReverseStack)
{
	if (!ReverseStackText) return;

	if (ReverseStack > 0)
	{
		ReverseStackText->SetText(FText::FromString(FString::Printf(TEXT("Reverse Debuff : %d"), ReverseStack)));
		SetVisibility(ESlateVisibility::Visible);
		ReverseStackText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		ReverseStackText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
