// CHEN

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent; 
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

public:	
	ASTUBaseWeapon();

	virtual void StartFire();
	virtual void StopFire();

	void Reload();
	bool IsAmmoFullPublic() const;
	bool IsClipZero()const;

	void TryToAddAmmo(int32 ClipAmount,bool& IsNeeaAdd);

	FWeaponUIData GetUIData()const { return UIData; }

	FAmmoData GetWeaponAmmo()const { return CurrentAmmo; }

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo {15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* MuzzleFX;

	virtual void BeginPlay() override;
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController()const;
	bool GetplayerViewPoint(FVector& ViewLocaton, FRotator& ViewRotator) const;
	FVector GetMuzzleWorldLocation()const;
	void MakeHit(FHitResult& HITRESULT, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreaseAmmo();
	bool IsAmmoFull() const;
	bool IsAmmoAndClipsFull() const;
	bool IsAmmoEmpty()const;
	bool IsClipEmpty()const;
	void ChangeClip();
	void LogAmmo();

	UNiagaraComponent* SpawnMuzzleFX();

private:
	FAmmoData CurrentAmmo;
};
