// CHEN


#include "Animations/STUReloadFinishedAnimNotify.h"

void USTUReloadFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);

}
