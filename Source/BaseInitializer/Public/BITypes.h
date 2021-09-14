#pragma once


enum class EBICharacterInit : uint8
{
	CI_Pawn = 0,
	CI_Controller = 1,
	CI_PlayerState = 2,
	CI_VoxelWorld = 3,
};

struct FBICharacterInitialization
{
	FBICharacterInitialization()
		: RequiredInitializers({
		{ (uint8)EBICharacterInit::CI_Pawn, false },
		{ (uint8)EBICharacterInit::CI_Controller, false },
		{ (uint8)EBICharacterInit::CI_PlayerState, false },
		{ (uint8)EBICharacterInit::CI_VoxelWorld, false }
		})
		, bLocalInit(false)
		, bRemoteInit(false)
	{}

protected:
	TMap<uint8, bool> RequiredInitializers;
	bool bLocalInit;
	bool bRemoteInit;

public:
	FORCEINLINE bool IsInitialized() const
	{
		return bLocalInit && bRemoteInit;
	}

	/** Called when RPC notifies it is initialized, for standalone builds set when local is initialized */
	void SetRemoteInit()
	{
		bRemoteInit = true;
	}

	/** @return True if initialized locally */
	bool Init(uint8 Initializer)
	{
		if (IsInitialized())
		{
			return false;
		}

		if (RequiredInitializers.Contains(Initializer))
		{
			RequiredInitializers[Initializer] = true;
			
			bLocalInit = true;
			for (auto& Init : RequiredInitializers)
			{
				if (!(bool)Init.Value)
				{
					bLocalInit = false;
					break;
				}
			}
		}
		else
		{
			const FString ErrorMsg = FString::Printf(TEXT("Provided Initializer %d does not exist"), Initializer);
			FMessageLog MsgLog(TEXT("PlayInEditor"));
			MsgLog.Warning(FText::FromString(ErrorMsg));
			MsgLog.Open(EMessageSeverity::Error);
		}

		return bLocalInit;
	}

	void Reset()
	{
		bLocalInit = false;
		bRemoteInit = false;
		for (auto& Init : RequiredInitializers)
		{
			Init.Value = false;
		}
	}
};