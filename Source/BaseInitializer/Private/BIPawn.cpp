// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.


#include "BIPawn.h"
#include "AIController.h"
#include "GameFramework/MovementComponent.h"

#include "VoxelWorld.h"

#include "BIPlayerController.h"
#include "BIPlayerCameraManager.h"
#include "BIHUD.h"
#include "BIPlayerState.h"


ABIPawn::ABIPawn()
{
	bInitCalled = false;
}

void ABIPawn::Init()
{
	if (bInitCalled) { return; }
	bInitCalled = true;

	OnInit();
	K2_OnInit();

	// Player State
	ABIPlayerState* const PS = (GetPlayerState()) ? GetPlayerState<ABIPlayerState>() : nullptr;
	if (PS) { PS->Init(); }

	// Player Controller
	ABIPlayerController* const PC = (GetController()) ? Cast<ABIPlayerController>(GetController()) : nullptr;
	if (PC) { PC->Init(); }

	// Player Camera Manager
	ABIPlayerCameraManager* const PM = (PC && PC->PlayerCameraManager) ? Cast<ABIPlayerCameraManager>(PC->PlayerCameraManager) : nullptr;
	if (PM) { PM->Init(); }

	// Player HUD
	ABIHUD* const HUD = (PC && PC->GetHUD()) ? PC->GetHUD<ABIHUD>() : nullptr;
	if (HUD) { HUD->Init(); }
}

void ABIPawn::OnInit()
{
	SetActorEnableCollision(true);

	SetActorHiddenInGame(false);
}

void ABIPawn::OnLocalInit()
{
	if (bInitCalled) { return; }

	if (GetNetMode() == NM_Standalone || GetLocalRole() == ROLE_SimulatedProxy)
	{
		Initializer.SetRemoteInit();
		Init();
	}
	else if (GetNetMode() == NM_Client)
	{
		NotifyServerInit();
	}
	else if (GetLocalRole() == ROLE_Authority)
	{
		NotifyClientInit();
	}

	if (Initializer.IsInitialized())
	{
		Init();
	}
}

void ABIPawn::OnRemoteInit()
{
	Initializer.SetRemoteInit();

	if (Initializer.IsInitialized())
	{
		Init();
	}
}

void ABIPawn::NotifyServerInit_Implementation()
{
	OnRemoteInit();
}

void ABIPawn::NotifyClientInit_Implementation()
{
	OnRemoteInit();
}

void ABIPawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorEnableCollision(false);

	if (Initializer.Init((uint8)EBICharacterInit::CI_Pawn))
	{
		OnLocalInit();
	}

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Controllers aren't replicated to simulated proxies
		if (Initializer.Init((uint8)EBICharacterInit::CI_Controller))
		{
			OnLocalInit();
		}
	}

	// Handle voxel world initialization (don't want to start doing anything if the world doesn't exit yet)
	AVoxelWorld* const VoxelWorld = GetVoxelWorld();
	if (VoxelWorld)
	{
		if (VoxelWorld->IsLoaded())
		{
			OnVoxelWorldLoaded();
		}
		else
		{
			VoxelWorld->OnWorldLoaded.AddDynamic(this, &ABIPawn::OnVoxelWorldLoaded);
		}
	}
	else
	{
		OnVoxelWorldLoaded();
	}
}

void ABIPawn::PossessedBy(AController* NewController)
{
	// This provides both player state and controller for AUTHORITY
	Super::PossessedBy(NewController);

	const bool bAIWithoutPlayerState = (NewController && Cast<AAIController>(NewController) && !Cast<AAIController>(NewController)->bWantsPlayerState);

	check(NewController || GetLocalRole() == ROLE_SimulatedProxy);
	check(GetPlayerState() || bAIWithoutPlayerState);

	if (Initializer.Init((uint8)EBICharacterInit::CI_Controller))
	{
		OnLocalInit();
	}

	if (Initializer.Init((uint8)EBICharacterInit::CI_PlayerState))
	{
		OnLocalInit();
	}

	if (!bAIWithoutPlayerState)
	{
		OnChangePlayerState();
	}

	OnChangeController();
}

void ABIPawn::UnPossessed()
{
	// Called on server
	Super::UnPossessed();

	OnChangeController();
}

void ABIPawn::OnRep_Controller()
{
	// This provides controller for CLIENT
	Super::OnRep_Controller();

	if (Initializer.Init((uint8)EBICharacterInit::CI_Controller))
	{
		OnLocalInit();
	}

	OnChangeController();
}

void ABIPawn::OnRep_PlayerState()
{
	// This provides player state for CLIENT
	Super::OnRep_PlayerState();

	if (Initializer.Init((uint8)EBICharacterInit::CI_PlayerState))
	{
		OnLocalInit();
	}

	OnChangePlayerState();
}

void ABIPawn::OnVoxelWorldLoaded()
{
	// Remove event delegate
	AVoxelWorld* const VoxelWorld = GetVoxelWorld();
	if (VoxelWorld && VoxelWorld->OnWorldLoaded.IsBound())
	{
		VoxelWorld->OnWorldLoaded.RemoveDynamic(this, &ABIPawn::OnVoxelWorldLoaded);
	}

	// Notify initialized voxel world
	if (Initializer.Init((uint8)EBICharacterInit::CI_VoxelWorld))
	{
		OnLocalInit();
	}
}
