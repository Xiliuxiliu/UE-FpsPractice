// CHEN


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon//Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponRifleLog, All, All)

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimerBetweenShots, true);
	UE_LOG(WeaponRifleLog, Display, TEXT("Start Fire Rifle"));
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ASTURifleWeapon::MakeShot()
{
	UE_LOG(LogTemp, Display, TEXT("Rifle MakeShot"));
	if (!GetWorld() || IsAmmoEmpty())
	{	
		UE_LOG(LogTemp, Display, TEXT("Rifle MakeShot Failed 1"));
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		UE_LOG(LogTemp, Display, TEXT("Rifle MakeShot Failed 2"));
		StopFire();
		return;
	}

	SpawnMuzzleFX();

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0F);
		//UE_LOG(LogBaseWeapon, Display, TEXT("Bones %s"), *HitResult.BoneName.ToString());
		WeaponFXComponent->PlayImpactFX(HitResult);

	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector & TraceStart, FVector & TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetplayerViewPoint(ViewLocation, ViewRotation))return false;

	//UE_LOG(LogBaseWeapon, Error, TEXT("ViewLocation is %s, ViewRotation is %s"), *ViewLocation.ToString(), *ViewRotation.ToString());

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	//UE_LOG(LogBaseWeapon, Error, TEXT("TraceStart is %s, TraceEnd is %s"), *TraceStart.ToString(), *TraceEnd.ToString());
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();//actor?
	if (!DamageActor)return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent) 
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if (MuzzleFXComponent) 
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible, true);
	}
}
