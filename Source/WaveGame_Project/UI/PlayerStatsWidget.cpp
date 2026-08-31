#include "UI/PlayerStatsWidget.h"
#include "PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UPlayerStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		PlayerCharacter->OnHPChanged.AddDynamic(this, &UPlayerStatsWidget::UpdateHP);
		UpdateHP(PlayerCharacter->GetHeath(), PlayerCharacter->GetMaxHealth());
		PlayerCharacter->OnSprintChanged.AddDynamic(this, &UPlayerStatsWidget::UpdateSprint);
		UpdateSprint(PlayerCharacter->GetCurSprintRate(), PlayerCharacter->GetMaxSprintRate());
	}
}

void UPlayerStatsWidget::UpdateHP(float CurHP, float MaxHP)
{
	if (!HPText || !HPBar) return;

	HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurHP, MaxHP)));
	HPBar->SetPercent(CurHP / MaxHP);
}

void UPlayerStatsWidget::UpdateSprint(float CurSprintRate, float MaxSprintRate)
{
	if (!SprintBar) return;

	SprintBar->SetPercent(CurSprintRate / MaxSprintRate);
}
