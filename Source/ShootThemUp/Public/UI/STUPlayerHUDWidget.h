// CHEN

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

/*class USTUWeaponComponent;
class USTUHealthComponent;*/

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent()const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData)const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData)const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsplayerAlive()const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsplayerSpectating()const;

private:
	/*
	* USTUWeaponComponent* GetWeaponComponent()const;
	* USTUHealthComponent* GetHealthComponent()const;
	*/
	
};
