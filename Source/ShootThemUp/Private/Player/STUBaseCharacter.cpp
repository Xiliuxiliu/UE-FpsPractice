// CHEN

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include <sstream>
#include "Camera/CameraComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& Object)
	:Super(Object.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent1 = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent1");
	SpringArmComponent1->SetupAttachment(GetRootComponent());
	SpringArmComponent1->bUsePawnControlRotation = true;
	SpringArmComponent1->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent1 = CreateDefaultSubobject<UCameraComponent>("CameraComponent1");
	CameraComponent1->SetupAttachment(SpringArmComponent1);

	STUHealthComponent1 = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");
	
	HealthTextComponent1 = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent1");
	HealthTextComponent1->SetupAttachment(GetRootComponent());
	HealthTextComponent1->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), STUHealthComponent1->GetHealth())));
	HealthTextComponent1->SetOwnerNoSee(true);

	STUWeaponComponent1 = CreateDefaultSubobject<USTUWeaponComponent>("STUWeaponComponent1");
	
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	check(STUHealthComponent1);
	check(HealthTextComponent1);
	check(GetCharacterMovement());

	STUHealthComponent1->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	STUHealthComponent1->OnHealthChanged.AddUObject(this,&ASTUBaseCharacter::OnHealthChanged);
	OnHealthChanged(STUHealthComponent1->GetHealth());

	LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);

}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(STUWeaponComponent1);
	check(GetMesh());

	PlayerInputComponent->BindAxis("MoveForward",this,&ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStratRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, STUWeaponComponent1, &USTUWeaponComponent::StartFire);

	PlayerInputComponent->BindAction("Fire", IE_Released, STUWeaponComponent1, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, STUWeaponComponent1, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, STUWeaponComponent1, &USTUWeaponComponent::Reload);
}

bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees:Degrees *FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{

	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f)return;
	AddMovementInput(GetActorForwardVector(),Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f)return;
	AddMovementInput(GetActorRightVector(), Amount);
}



void ASTUBaseCharacter::OnStratRunning()
{
	WantsToRun = true;
}
void ASTUBaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}

void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Character %s is dead"), *GetName());

	STUWeaponComponent1->StopFire();
	//PlayAnimMontage(DeathAnimMontage1);

	//停止角色运动，但可以进行旋转
	GetCharacterMovement()->DisableMovement();

	//停止角色控制输入
	DisableInput(UGameplayStatics::GetPlayerController(this,0));
	SetLifeSpan(5.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent1->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
	
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	UE_LOG(BaseCharacterLog, Display, TEXT("character OnLand Z-Speed is %f"), FallVelocityZ);

	if (FallVelocityZ < LandDamageVelocity.X)
	{
		return;
	}
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandDamageVelocity, LandDamage, FallVelocityZ);
	UE_LOG(BaseCharacterLog, Display, TEXT("Final Damage is %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{},nullptr,nullptr);
}
