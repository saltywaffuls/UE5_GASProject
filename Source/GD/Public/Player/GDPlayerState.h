// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GD_API AGDPlayerState : public APlayerState , public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGDPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UGDAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);


	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetArmor() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "PixalPimp|GDPlayerState|Attributes")
	int32 GetCharacterLevel() const;

protected:

	UPROPERTY()
	class UGDAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UGDAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
