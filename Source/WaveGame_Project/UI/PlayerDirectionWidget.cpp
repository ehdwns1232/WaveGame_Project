#include "UI/PlayerDirectionWidget.h"
#include "PlayerCharacter.h"

void UPlayerDirectionWidget::NativeConstruct()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		PlayerCharacter->OnDirectionChanged.AddDynamic(this, &UPlayerDirectionWidget::UpdateDirection);
	}
}

void UPlayerDirectionWidget::UpdateDirection(FVector2D MoveInput)
{
	bool Up = MoveInput.X >= 0.1f;
	bool PreviousUp = PreviousMoveInput.X >= 0.1f;

	if (Up != PreviousUp)
	{
		if (Up)
		{
			PlayAnimation(DirUpAnim, 0.0f, 0);
		}
		else
		{
			StopAnimation(DirUpAnim);
		}
	}

	bool Down = MoveInput.X <= -0.1f;
	bool PreviousDown = PreviousMoveInput.X <= -0.1f;

	if (Down != PreviousDown)
	{
		if (Down)
		{
			PlayAnimation(DirDownAnim, 0.0f, 0);
		}
		else
		{
			StopAnimation(DirDownAnim);
		}
	}

	bool Right = MoveInput.Y >= 0.1f;
	bool PreviousRight = PreviousMoveInput.Y >= 0.1f;

	if (Right != PreviousRight)
	{
		if (Right)
		{
			PlayAnimation(DirRightAnim, 0.0f, 0);
		}
		else
		{
			StopAnimation(DirRightAnim);
		}
	}

	bool Left = MoveInput.Y <= -0.1f;
	bool PreviousLeft = PreviousMoveInput.Y <= -0.1f;

	if (Left != PreviousLeft)
	{
		if (Left)
		{
			PlayAnimation(DirLeftAnim, 0.0f, 0);
		}
		else
		{
			StopAnimation(DirLeftAnim);
		}
	}

	PreviousMoveInput = MoveInput;
}
