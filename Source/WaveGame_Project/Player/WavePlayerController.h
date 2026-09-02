#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WavePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UHUDWidget;
class UMainMenuWidget;

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
	UHUDWidget* GetHUDWidget() const;
	UFUNCTION(BlueprintPure, Category = "UI|Menu")
	UMainMenuWidget* GetMainMenuWidget() const;
	UFUNCTION(BlueprintCallable, Category = "UI|Menu")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "UI|Menu")
	void ShowMainMenu(bool bIsRestart, bool bIsClear);
	UFUNCTION(BlueprintCallable, Category = "UI|Menu")
	void StartGame();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultIMC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> PickupAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|HUD")
	TSubclassOf<UHUDWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUD")
	TObjectPtr<UHUDWidget> HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Menu")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Menu")
	TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance;
	
};
