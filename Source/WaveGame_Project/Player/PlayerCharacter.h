#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UENUM(BlueprintType, meta = (Bitflags))
enum class EPlayerDebuffState : uint8
{
	None = 0 UMETA(DisplayName = "1. None"),
	Slow = 1 UMETA(DisplayName = "2. Slow"),
	Reverse = 2 UMETA(DisplayName = "3. Reverse"),
	SlowAndReverse = 3 UMETA(DisplayName = "4. SlowAndReverse")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSprintChanged, float, CurSprintRate, float, MaxSprintRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlowStackChanged, int32, SlowStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReverseStackChanged, int32, ReverseStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectionChanged, FVector2D, MoveInput);

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
	float GetHeath() const;
	float GetMaxHealth() const;
	float GetCurSprintRate() const;
	float GetMaxSprintRate() const;
	int32 GetSlowStack() const;
	int32 GetReverseStack() const;

public:
	void AddHealth(float Amount);
	void AddStamina(float Amount);
	void OnDeath();
	void AddSlowStack(float SlowTime);
	void AddReverseStack(float ReverseTime);
	void ApplySlow();
	void RemoveSlow();
	void RemoveReverse();
	void MaintainSlow(float SlowTime);
	void AddPlayerDebuffState(EPlayerDebuffState NewState);
	void RemovePlayerDebuffState(EPlayerDebuffState RemoveState);
	void MaintainReverse(float ReverseTime);
	void SetSprintSpeed(float NewSpeed);
	void RemoveAllDebuff();

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
	UFUNCTION()
	void Pickup(const FInputActionValue& Value);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	
protected:
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	uint8 CurrentPlayerState = static_cast<uint8>(EPlayerDebuffState::None);

public:
	UPROPERTY(BlueprintAssignable)
	FOnHPChanged OnHPChanged;
	UPROPERTY(BlueprintAssignable)
	FOnSprintChanged OnSprintChanged;
	UPROPERTY(BlueprintAssignable)
	FOnSlowStackChanged OnSlowStackChanged;
	UPROPERTY(BlueprintAssignable)
	FOnReverseStackChanged OnReverseStackChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDirectionChanged OnDirectionChanged;
	
private:
	FTimerHandle SlowTimerHandle;
	FTimerHandle ReverseTimerHandle;
	FTimerHandle SprintTimerHandle;

	float NormalSpeed = 600.0f;
	float SpeedMultipiler = 1.7f;
	float SprintSpeed = NormalSpeed * SpeedMultipiler;
	float MaxSprintRate = 10.0f;
	float CurrentSprintRate = MaxSprintRate;
	float SprintRecovery = 1.0f;

	bool bIsSprinting = false;
};
