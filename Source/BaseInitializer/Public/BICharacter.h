// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BITypes.h"
#include "BICharacter.generated.h"

class AVoxelWorld;

UCLASS()
class BASEINITIALIZER_API ABICharacter : public ACharacter
{
	GENERATED_BODY()

private:
	FBICharacterInitialization Initializer;

	bool bInitCalled;

public:
	ABICharacter(const FObjectInitializer& OI);

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE bool IsInitialized() const { return Initializer.IsInitialized(); }

protected:
	void Init();

	/** 
	 * Called when fully initialized
	 * i.e Has PlayerState & Controller & Any other required setup 
	 * And they exist ON BOTH SERVER AND CLIENT
	 */
	virtual void OnInit();

	/** 
	 * Called when fully initialized
	 * i.e Has PlayerState & Controller & Any other required setup 
	 * And they exist ON BOTH SERVER AND CLIENT
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = Character, meta = (DisplayName = "On Init"))
	void K2_OnInit();

	void OnLocalInit();

	void OnRemoteInit();

	UFUNCTION(Server, Reliable)
	void NotifyServerInit();

	UFUNCTION(Client, Reliable)
	void NotifyClientInit();

	virtual void BeginPlay() override;

	// This provides both player state and controller for AUTHORITY
	virtual void PossessedBy(AController* NewController) override final;

	virtual void UnPossessed() override final;

	// This provides controller for CLIENT
	virtual void OnRep_Controller() override final;

	// This provides player state for CLIENT
	virtual void OnRep_PlayerState() override final;

	virtual void OnChangeController() {}
	virtual void OnChangePlayerState() {}

	UFUNCTION()
	void OnVoxelWorldLoaded();

	/** Must override in subclass */
	FORCEINLINE virtual AVoxelWorld* GetVoxelWorld() const { return nullptr; }
};
