// CHEN


#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent1 = CreateDefaultSubobject<USceneComponent>("SceneComponent1");
	SetRootComponent(SceneComponent1);
}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

	//ApplyRadialDamage伤害功能会被Block阻挡
	UGameplayStatics::ApplyRadialDamage(this, Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
}