#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WavePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class WAVEGAME_PROJECT_API AWavePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWavePlayerController();

protected:
	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultIMC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
};
