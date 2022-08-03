// Fill out your copyright notice in the Description page of Project Settings.


#include "GDAssetManager.h"
#include "AbilitySystemGlobals.h"

void UGDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	UE_LOG(LogTemp, Warning, TEXT("Hello World!!!"));
}