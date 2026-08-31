#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "WavePlayerController.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "WaveGameState.h"
#include "UI/HUDWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bDoCollisionTest = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	switch (CurrentPlayerState)
	{
	case static_cast<uint8>(EPlayerDebuffState::None):
		break;
	case static_cast<uint8>(EPlayerDebuffState::Slow):
		ApplySlow();
		break;
	case static_cast<uint8>(EPlayerDebuffState::Reverse):
		AddPlayerDebuffState(EPlayerDebuffState::Reverse);
		break;
	case static_cast<uint8>(EPlayerDebuffState::SlowAndReverse):
	{
		ApplySlow();
		AddPlayerDebuffState(EPlayerDebuffState::Reverse);
		break;
	}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting)
	{
		CurrentSprintRate -= DeltaTime;
		OnSprintChanged.Broadcast(CurrentSprintRate, MaxSprintRate);
		if (CurrentSprintRate <= 0.0f)
		{
			CurrentSprintRate = 0.0f;
			StopSprint(0);
		}
	}
	else
	{
		if (CurrentSprintRate < MaxSprintRate)
		{
			CurrentSprintRate += (SprintRecovery * DeltaTime);
			OnSprintChanged.Broadcast(CurrentSprintRate, MaxSprintRate);
			if (CurrentSprintRate > MaxSprintRate)
			{
				CurrentSprintRate = MaxSprintRate;
				SetActorTickEnabled(false);
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AWavePlayerController* PlayerController = GetController<AWavePlayerController>())
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::Move);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
			}
		}
	}
}
                        
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	OnHPChanged.Broadcast(Health, MaxHealth);
	if (Health <= 0.0f)
	{
		OnDeath();
	}


	return ActualDamage;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	FVector2D MoveInput = Value.Get<FVector2D>();

	if (CurrentPlayerState & static_cast<uint8>(EPlayerDebuffState::Reverse)) MoveInput *= -1;
	
	OnDirectionChanged.Broadcast(MoveInput);

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	Jump();
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp && CurrentSprintRate > 0.1f)
	{
		SetActorTickEnabled(true);
		bIsSprinting = true;
		MovementComp->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		if (CurrentPlayerState == (uint8)EPlayerDebuffState::Slow)
		{
			MovementComp->MaxWalkSpeed = NormalSpeed / SlowDivideValue;
		}
		else
		{
			MovementComp->MaxWalkSpeed = NormalSpeed;
		}
	}
}

float APlayerCharacter::GetHeath() const
{
	return Health;
}

float APlayerCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

float APlayerCharacter::GetCurSprintRate() const
{
	return CurrentSprintRate;
}

float APlayerCharacter::GetMaxSprintRate() const
{
	return MaxSprintRate;
}

int32 APlayerCharacter::GetSlowStack() const
{
	return SlowStack;
}

int32 APlayerCharacter::GetReverseStack() const
{
	return ReverseStack;
}

void APlayerCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	OnHPChanged.Broadcast(Health, MaxHealth);
}

void APlayerCharacter::OnDeath()
{
	AWaveGameState* WaveGameState = Cast<AWaveGameState>(GetWorld()->GetGameState());
	if (WaveGameState)
	{
		WaveGameState->OnGameOver();
	}
}

void APlayerCharacter::AddSlowStack(float SlowTime)
{
	SlowStack++;
	OnSlowStackChanged.Broadcast(SlowStack);
	if (SlowStack == 1 && CurrentPlayerState != (uint8)EPlayerDebuffState::Slow)
	{
		ApplySlow();
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &APlayerCharacter::MaintainSlow, SlowTime);
		GetWorldTimerManager().SetTimer(SlowTimerHandle, Delegate, SlowTime, false);
	}
}

void APlayerCharacter::AddReverseStack(float ReverseTime)
{
	ReverseStack++;
	OnReverseStackChanged.Broadcast(ReverseStack);
	if (ReverseStack == 1 && CurrentPlayerState != (uint8)EPlayerDebuffState::Reverse)
	{
		AddPlayerDebuffState(EPlayerDebuffState::Reverse);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &APlayerCharacter::MaintainReverse, ReverseTime);
		GetWorldTimerManager().SetTimer(ReverseTimerHandle, Delegate, ReverseTime, false);
	}
}

void APlayerCharacter::ApplySlow()
{
	AddPlayerDebuffState(EPlayerDebuffState::Slow);
	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		CharacterMovementComp->MaxWalkSpeed = NormalSpeed / SlowDivideValue;
		SetSprintSpeed(CharacterMovementComp->MaxWalkSpeed);
	}
}

void APlayerCharacter::RemoveSlow()
{
	RemovePlayerDebuffState(EPlayerDebuffState::Slow);
	SlowStack = 0;
	OnSlowStackChanged.Broadcast(SlowStack);
	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		CharacterMovementComp->MaxWalkSpeed = NormalSpeed;
		SetSprintSpeed(CharacterMovementComp->MaxWalkSpeed);
	}
}

void APlayerCharacter::RemoveReverse()
{
	RemovePlayerDebuffState(EPlayerDebuffState::Reverse);
	ReverseStack = 0;
	OnReverseStackChanged.Broadcast(ReverseStack);
}

void APlayerCharacter::MaintainSlow(float SlowTime)
{
	SlowStack--;
	OnSlowStackChanged.Broadcast(SlowStack);
	if (SlowStack > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &APlayerCharacter::MaintainSlow, SlowTime);
		GetWorldTimerManager().SetTimer(SlowTimerHandle, Delegate, SlowTime, false);
	}
	else
	{
		RemoveSlow();
		RemovePlayerDebuffState(EPlayerDebuffState::Slow);
	}
}

void APlayerCharacter::AddPlayerDebuffState(EPlayerDebuffState NewState)
{
	CurrentPlayerState |= static_cast<uint8>(NewState);
}

void APlayerCharacter::RemovePlayerDebuffState(EPlayerDebuffState RemoveState)
{
	CurrentPlayerState &= ~static_cast<uint8>(RemoveState);
}

void APlayerCharacter::MaintainReverse(float ReverseTime)
{
	ReverseStack--;
	OnReverseStackChanged.Broadcast(ReverseStack);
	if (ReverseStack > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &APlayerCharacter::MaintainReverse, ReverseTime);
		GetWorldTimerManager().SetTimer(ReverseTimerHandle, Delegate, ReverseTime, false);
	}
	else
	{
		RemovePlayerDebuffState(EPlayerDebuffState::Reverse);
	}
}

void APlayerCharacter::SetSprintSpeed(float NewSpeed)
{
	SprintSpeed = NewSpeed * SpeedMultipiler;
}

void APlayerCharacter::RemoveAllDebuff()
{
	RemoveSlow();
	RemoveReverse();
}



