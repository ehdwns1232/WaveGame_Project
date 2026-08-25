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
	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	UUserWidget* GetHUDWidget() const;
	UFUNCTION(BlueprintPure, Category = "UI|Menu")
	UUserWidget* GetMainMenuWidget() const;
	UFUNCTION(BlueprintCallable, Category = "UI|Menu")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "UI|Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "UI|Menu")
	void StartGame();
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Menu")
	UUserWidget* MainMenuWidgetInstance;

};
