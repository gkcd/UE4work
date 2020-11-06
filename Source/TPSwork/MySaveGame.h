// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TPSWORK_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()


public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    TArray<int32> ScoreList;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    TArray<FString> NameList;


    UMySaveGame();
	
};
