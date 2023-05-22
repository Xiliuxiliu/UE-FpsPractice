// CHEN


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& Object):Super(Object)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASTUAIController::StaticClass();
}
