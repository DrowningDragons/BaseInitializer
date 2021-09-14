// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.


#include "BIPlayerController.h"

ABIPlayerController::ABIPlayerController()
{
	DisableInput(this);
}

void ABIPlayerController::Init()
{
	OnInit();
	K2_OnInit();
}

void ABIPlayerController::OnInit()
{
	EnableInput(this);
}
