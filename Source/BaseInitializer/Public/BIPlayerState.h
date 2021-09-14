// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BIPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASEINITIALIZER_API ABIPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	friend class ABIPawn;
	friend class ABICharacter;
protected:
	void Init();

	/**
	 * Called when fully initialized
	 * i.e Has PlayerState & Controller & Any other required setup
	 * And they exist ON BOTH SERVER AND CLIENT
	 */
	virtual void OnInit() {}

	/**
	 * Called when fully initialized
	 * i.e Has PlayerState & Controller & Any other required setup
	 * And they exist ON BOTH SERVER AND CLIENT
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = Character, meta = (DisplayName = "On Init"))
	void K2_OnInit();
};
