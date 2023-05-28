// CHEN

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUAIFireService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIFireService : public UBTService
{
	GENERATED_BODY()

public:
	USTUAIFireService();

protected:
	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	//FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
