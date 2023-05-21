// CHEN


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon//Components/STUWeaponFXComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponents = CreateDefaultSubobject<USphereComponent>("CollisionComponents");
	CollisionComponents->InitSphereRadius(5.0f);
	CollisionComponents->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponents->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponents->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponents);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponents->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponents->IgnoreActorWhenMoving(this, true);
	CollisionComponents->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);

}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	//MakeDamage
	UGameplayStatics::ApplyRadialDamage(this, //
		DamageAmount,//
		GetActorLocation(),//
		DamageRadius, //
		UDamageType::StaticClass(), //
		{GetOwner()}, //
		this, //
		GetController(),
		DoFullDamage);

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, LifeSeconds);
	WeaponFXComponent->PlayImpactFX(Hit);
	Destroy();
}

AController* ASTUProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

