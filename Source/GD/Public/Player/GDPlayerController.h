// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Characters/GDCharacterBase.h"
#include "GDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GD_API AGDPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:

	// Server only
	virtual void OnPossess(APawn* InPawn) override;
	
};
