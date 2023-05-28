// CHEN


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh1");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("Strat Fire base"));
}

void ASTUBaseWeapon::StopFire()
{

}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count <= 0"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count <= 0"));

	CurrentAmmo = DefaultAmmo;
	
}

void ASTUBaseWeapon::MakeShot()
{

}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player)return nullptr;
	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetplayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller1 = GetPlayerController();

	if (!Controller1)return false;
	Controller1->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	return true;

}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetplayerViewPoint(ViewLocation, ViewRotation)) 
	{ 
		UE_LOG(LogBaseWeapon, Display, TEXT("GetTraceData Failed 1"));
		return false; 
	}

	UE_LOG(LogBaseWeapon, Error, TEXT("ViewLocation is %s, ViewRotation is %s"), *ViewLocation.ToString(), *ViewRotation.ToString());

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	UE_LOG(LogBaseWeapon, Error, TEXT("TraceStart is %s, TraceEnd is %s"), *TraceStart.ToString(), *TraceEnd.ToString());
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{

	FCollisionQueryParams CollisiobParams;
	CollisiobParams.AddIgnoredActor(GetOwner());
	CollisiobParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisiobParams);
}

void ASTUBaseWeapon::Reload()
{
	if (IsAmmoFull()) return;
	if (CurrentAmmo.Infinite ==false && CurrentAmmo.Clips == 0)return;
	ChangeClip();
}

bool ASTUBaseWeapon::IsAmmoFullPublic() const
{
	return IsAmmoFull();
}

bool ASTUBaseWeapon::IsClipZero() const
{
	return CurrentAmmo.Infinite == false && CurrentAmmo.Clips == 0;
}

void ASTUBaseWeapon::TryToAddAmmo(int32 ClipAmount,bool& IsNeedAdd)
{
	if (CurrentAmmo.Infinite == true || IsAmmoAndClipsFull() || ClipAmount <= 0) 
	{
		return;
	}
	else if (!CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(ClipAmount, 0, DefaultAmmo.Clips + 1);
		IsNeedAdd = true;
		UE_LOG(LogBaseWeapon, Display, TEXT("Add All"));
	}
	else if(CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipAmount;
		if (DefaultAmmo.Clips - NextClipsAmount >= 0) 
		{
			CurrentAmmo.Clips = NextClipsAmount;
			IsNeedAdd = true;
			UE_LOG(LogBaseWeapon, Display, TEXT("Add clips"));
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
			IsNeedAdd = true;
			UE_LOG(LogBaseWeapon, Display, TEXT("Add clips and bullets"));
		}
	}
	else if(CurrentAmmo.Clips >= DefaultAmmo.Clips)
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		IsNeedAdd = true;
		UE_LOG(LogBaseWeapon, Display, TEXT("Add Bullets"));
	}
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	CurrentAmmo.Bullets--;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	//return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
	return IsClipEmpty();
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::IsAmmoAndClipsFull() const
{
	return CurrentAmmo.Bullets == DefaultAmmo.Bullets &&
		CurrentAmmo.Clips == DefaultAmmo.Clips;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	if (!CurrentAmmo.Infinite) 
	{
		CurrentAmmo.Clips--;
		UE_LOG(LogBaseWeapon, Display, TEXT("----------Change Clip---------"));
	}
}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleFX,//
		WeaponMesh,//
		MuzzleSocketName,//
		FVector::ZeroVector,//
		FRotator::ZeroRotator,//
		EAttachLocation::SnapToTarget,//
		true);
}
