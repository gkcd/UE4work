// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSworkGameMode.generated.h"

UCLASS(minimalapi)
class ATPSworkGameMode : public AGameModeBase
{
	GENERATED_BODY()

		virtual void StartPlay() override;

public:
	ATPSworkGameMode();
	int Score = 0;
};



