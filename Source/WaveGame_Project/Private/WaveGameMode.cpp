#include "WaveGameMode.h"
#include "PlayerCharacter.h"
#include "WavePlayerController.h"
#include "WaveGameState.h"

AWaveGameMode::AWaveGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AWavePlayerController::StaticClass();
	GameStateClass = AWaveGameState::StaticClass();
}
