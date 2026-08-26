#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerDebuffState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Slow = 1 UMETA(DisplayName = "Slow"),
	Reverse = 2 UMETA(DisplayName = "Reserve"),
	SlowAndReverse = 4 UMETA(DisplayName = "SlowAndReserve")
};

UCLASS()
class WAVEGAME_PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintPure, Category = "Heatlh")
	float GetHeath() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	
	void OnDeath();
	void AddSlowStack(float SlowTime);
	void AddReverseStack(float ReverseTime);
	void ApplySlow();
	void RemoveSlow();
	void MaintainSlow(float SlowTime);
	void AddPlayerDebuffState(EPlayerDebuffState NewState);
	void RemovPlayerDebuffState(EPlayerDebuffState RemoveState);
	void MaintainReverse(float ReverseTime);
	void SetSprintSpeed(float NewSpeed);
public:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	int32 SlowStack = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff", meta = (ClampMin = "1.5"))
	float SlowDivideValue = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	int32 ReverseStack = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsReverseDir = false;

private:
	FTimerHandle SlowTimerHandle;
	FTimerHandle ReverseTimerHandle;

	uint8 CurrentPlayerState = static_cast<uint8>(EPlayerDebuffState::None);
	float NormalSpeed = 600.0f;
	float SpeedMultipiler = 1.7f;
	float SprintSpeed = NormalSpeed * SpeedMultipiler;
};
