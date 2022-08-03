// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/GDPlayerCharacter.h"
#include "AI/GDPlayerAIController.h"
#include "Camera/CameraComponent.h"
#include "Characters/Abilities/AttributeSets/GDAttributeSetBase.h"
#include "Characters/Abilities/GDAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/GDPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GDPlayerController.h"
#include "Player/GDPlayerState.h"

AGDPlayerCharacter::AGDPlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 68.492264));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->FieldOfView = 80.0f;


	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));


	AIControllerClass = AGDPlayerAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

// Called to bind functionality to input
void AGDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGDPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGDPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AGDPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGDPlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis("Turn", this, &AGDPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGDPlayerCharacter::TurnRate);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

// Server only
void AGDPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		InitializeStartingValue(PS);

		// End respawn specific things


		AddStartupEffects();

		AddCharacterAbilities();

		/*AGDPlayerController* PC = Cast<AGDPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}*/

		//InitializeFloatingStatusBar();
	}
}

USpringArmComponent* AGDPlayerCharacter::GetCameraBoom()
{
	return CameraBoom;
}

UCameraComponent* AGDPlayerCharacter::GetFollowCamera()
{
	return FollowCamera;
}

float AGDPlayerCharacter::GetStartingCameraBoomArmLength()
{
	return StartingCameraBoomArmLength;
}

FVector AGDPlayerCharacter::GetStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}


/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/

void AGDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Only needed for Heroes placed in world and when the player is the Server.
	// On respawn, they are set up in PossessedBy.
	// When the player a client, the floating status bars are all set up in OnRep_PlayerState.
	//InitializeFloatingStatusBar();

	StartingCameraBoomArmLength = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();
}

void AGDPlayerCharacter::LookUp(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value);
	}
}

void AGDPlayerCharacter::LookUpRate(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	}
}

void AGDPlayerCharacter::Turn(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void AGDPlayerCharacter::TurnRate(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
	}
}

void AGDPlayerCharacter::MoveForward(float Value)
{
	if (IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AGDPlayerCharacter::MoveRight(float Value)
{
	if (IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}


// Client only
void AGDPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		InitializeStartingValue(PS);
		BindASCInput();
	}
}

//https://youtu.be/kUJhqRDf8Ms?t=1416
void AGDPlayerCharacter::InitializeStartingValue(AGDPlayerState* PS)
{

	// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
	AbilitySystemComponent = Cast<UGDAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	// Set the AttributeSetBase for convenience attribute functions
	AttributeSetBase = PS->GetAttributeSetBase();

	// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
	// For now assume possession = spawn/respawn.
	InitializeAttributes();


	// Respawn specific things that won't affect first possession.

	// Forcibly set the DeadTag count to 0
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
	SetHealth(GetMaxHealth());
	SetStamina(GetMaxStamina());

	UE_LOG(LogTemp, Warning, TEXT("InitializeStartingValue"));
}

void AGDPlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EGDAbilityInputID"), static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}