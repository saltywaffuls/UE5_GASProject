// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GDAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GD_API UGDAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;

};
