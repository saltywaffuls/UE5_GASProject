// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GDPlayerState.h"
#include "Characters/Abilities/AttributeSets/GDAttributeSetBase.h"
#include "Characters/Abilities/GDAbilitySystemComponent.h"


AGDPlayerState::AGDPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<UGDAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AGDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGDAttributeSetBase* AGDPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AGDPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AGDPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	/*AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	if (PC)
	{
		UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->ShowAbilityConfirmCancelText(ShowText);
		}
	}*/
}

float AGDPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AGDPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AGDPlayerState::GetStamina() const
{
	return AttributeSetBase->GetStamina();
}

float AGDPlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float AGDPlayerState::GetStaminaRegenRate() const
{
	return AttributeSetBase->GetStaminaRegenRate();
}

float AGDPlayerState::GetArmor() const
{
	return AttributeSetBase->GetArmor();
}

float AGDPlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

int32 AGDPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

void AGDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGDPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGDPlayerState::MaxHealthChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &AGDPlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &AGDPlayerState::MaxStaminaChanged);
		StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute()).AddUObject(this, &AGDPlayerState::StaminaRegenRateChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &AGDPlayerState::CharacterLevelChanged);

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGDPlayerState::StunTagChanged);
	}
}

void AGDPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	//float Health = Data.NewValue;

	//// Update floating status bar
	//AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetPawn());
	//if (Hero)
	//{
	//	UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
	//	if (HeroFloatingStatusBar)
	//	{
	//		HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
	//	}
	//}

	//// Update the HUD
	//// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

	//// If the player died, handle death
	//if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	//{
	//	if (Hero)
	//	{
	//		Hero->Die();
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT(" Health Changed !!! implement later"));
}

void AGDPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	//float MaxHealth = Data.NewValue;

	//// Update floating status bar
	//AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetPawn());
	//if (Hero)
	//{
	//	UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
	//	if (HeroFloatingStatusBar)
	//	{
	//		HeroFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
	//	}
	//}

	//// Update the HUD
	//AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	//if (PC)
	//{
	//	UGDHUDWidget* HUD = PC->GetHUD();
	//	if (HUD)
	//	{
	//		HUD->SetMaxHealth(MaxHealth);
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT(" Max Health Changed !!! implement later"));
}

void AGDPlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
	//float Stamina = Data.NewValue;

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
	UE_LOG(LogTemp, Warning, TEXT(" Stamina Changed !!! implement later"));
}

void AGDPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	//float MaxStamina = Data.NewValue;

	//// Update the HUD
	//AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	//if (PC)
	//{
	//	UGDHUDWidget* HUD = PC->GetHUD();
	//	if (HUD)
	//	{
	//		HUD->SetMaxStamina(MaxStamina);
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT(" Max Stamina Changed !!! implement later"));
}

void AGDPlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	//float StaminaRegenRate = Data.NewValue;

	//// Update the HUD
	//AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	//if (PC)
	//{
	//	UGDHUDWidget* HUD = PC->GetHUD();
	//	if (HUD)
	//	{
	//		HUD->SetStaminaRegenRate(StaminaRegenRate);
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT(" Stamina Regen Rate Changed !!! implement later"));
}

void AGDPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	//float CharacterLevel = Data.NewValue;

	//// Update the HUD
	//AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	//if (PC)
	//{
	//	UGDHUDWidget* HUD = PC->GetHUD();
	//	if (HUD)
	//	{
	//		HUD->SetHeroLevel(CharacterLevel);
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT(" Character Level Changed !!! implement later"));
}

void AGDPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}