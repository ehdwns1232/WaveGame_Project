#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WaveGameState.generated.h"


UCLASS()
class WAVEGAME_PROJECT_API AWaveGameState : public AGameState
{
	GENERATED_BODY()

public:
	AWaveGameState();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	void StartLevel();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score;

};
