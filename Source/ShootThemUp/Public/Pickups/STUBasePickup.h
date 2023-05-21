// CHEN

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	

	ASTUBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	float ReSpawnTime = 5.0f;

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

public:	

	virtual void Tick(float DeltaTime) override;

private:
	void PickupWasTaken();
	void Respawn();

	virtual bool GivePickupTo(APawn* PlayerPawn);

	float RotationYaw = 0.0f;
	void GenerateRotationYaw();

};
