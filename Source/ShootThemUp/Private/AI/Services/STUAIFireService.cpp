// CHEN


#include "AI/Services/STUAIFireService.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(STUAIFireLog,All,All)

USTUAIFireService::USTUAIFireService()
{
	NodeName = "Fire";
}

void USTUAIFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		UE_LOG(STUAIFireLog,Display,TEXT("STUAIFireLog enter"));
		const auto Controller = OwnerComp.GetAIOwner();
		if (!Controller) 
		{ 
			UE_LOG(STUAIFireLog, Display, TEXT("Controller miss")); 
			return; 
		}

		const auto Pawn = Controller->GetPawn();
		if (!Pawn)
		{
			UE_LOG(STUAIFireLog, Display, TEXT("Pawn miss"));
			return;
		}
		const auto Player = Cast<ASTUAICharacter>(Pawn);

		const auto WeaponComponent = STUUtils::STUGetPlayerComponent<USTUWeaponComponent>(Player);
		if (!WeaponComponent)
		{
			UE_LOG(STUAIFireLog, Display, TEXT("WeaoponComponent miss"));
			return;
		}

		WeaponComponent->StartFire();
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
