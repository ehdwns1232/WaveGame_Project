#include "Game/WaveGameMode.h"
#include "Player/PlayerCharacter.h"
#include "Player/WavePlayerController.h"
#include "Game/WaveGameState.h"

AWaveGameMode::AWaveGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AWavePlayerController::StaticClass();
	GameStateClass = AWaveGameState::StaticClass();
}
