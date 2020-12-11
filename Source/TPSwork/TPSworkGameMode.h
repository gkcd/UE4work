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


	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UFUNCTION(BlueprintCallable)
	void SaveGame(FString Name, int32 Score);

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	//UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UPROPERTY(EditAnywhere)
	float GameOverTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GM_NameList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GM_ScoreList;

};



