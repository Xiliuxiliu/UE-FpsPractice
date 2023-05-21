// CHEN

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;


UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& Object);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USTUHealthComponent* STUHealthComponent1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* HealthTextComponent1;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
		USTUWeaponComponent* STUWeaponComponent1;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* DeathAnimMontage1;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandDamageVelocity = FVector2D(900.0f,1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandDamage = FVector2D(10.0f,100.0f);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetMovementDirection() const;


private:
	int8 WantsToRun = false;
	int8 IsMovingForward = false;
	void OnStratRunning();
	void OnStopRunning();
	FVector Vector;
	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void OnDeath();
	void OnHealthChanged(float Health);
	
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

};
