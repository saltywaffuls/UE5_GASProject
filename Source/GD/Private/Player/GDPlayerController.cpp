// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GDPlayerController.h"
#include "AbilitySystemComponent.h"
#include "Player/GDPlayerState.h"

// Server only
void AGDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}