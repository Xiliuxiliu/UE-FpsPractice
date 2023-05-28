// CHEN

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"


class ASTUBaseWeapon;
class UAnimMontage;

/*USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* ReloadAnimMontage;

};
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData)const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData)const;

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly,Category = "Animation")
	UAnimMontage* EquipeAnimMontage;

	int32 CurrentWeaponIndex = 0;

	bool CanFire()const;
	bool CanEquip()const;
	void EquipWeapon(int32 WeaponIndex);

private:
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void PlayAnimMontage(UAnimMontage* AnimMontage);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	bool CanReload()const;

};
