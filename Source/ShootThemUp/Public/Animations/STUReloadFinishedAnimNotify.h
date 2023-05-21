// CHEN

#pragma once

#include "CoreMinimal.h"
#include "Animations/STUAnimNotify.h"
#include "STUReloadFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*)

UCLASS()
class SHOOTTHEMUP_API USTUReloadFinishedAnimNotify : public USTUAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;

	FOnNotifiedSignature OnNotified;
};
