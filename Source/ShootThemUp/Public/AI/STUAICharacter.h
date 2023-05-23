// CHEN

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
public:
	ASTUAICharacter(const FObjectInitializer& Object);

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
};
