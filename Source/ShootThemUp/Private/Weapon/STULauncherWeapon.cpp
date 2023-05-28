// CHEN


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponLauncherLog,All,All)

void ASTULauncherWeapon::StartFire()
{
	UE_LOG(WeaponLauncherLog, Display, TEXT("Strat Fire Launcher"));
	MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
	UE_LOG(WeaponLauncherLog, Display, TEXT("Enter Launcher MakeShot"));
	if (!GetWorld() || IsAmmoEmpty())
	{ 
		UE_LOG(WeaponLauncherLog, Display, TEXT("Launcher MakeShot Failed 1"));
		return; 
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) 
	{ 
		UE_LOG(WeaponLauncherLog, Display, TEXT("Launcher MakeShot Failed 2"));
		return; 
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator,GetMuzzleWorldLocation());
	ASTUProjectile*  Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass,SpawnTransform);
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
	SpawnMuzzleFX();
}
