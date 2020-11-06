// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSworkGameMode.h"
#include "TPSworkCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MySaveGame.h"
#include "Kismet/GamePlayStatics.h"

ATPSworkGameMode::ATPSworkGameMode()
{
    //set default pawn class to our Blueprinted character
  //  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_ThirdPersonCharacter"));
  //  if (PlayerPawnBPClass.Class != NULL)
  //  {
  //      DefaultPawnClass = PlayerPawnBPClass.Class;
  //  }

    SaveSlotName = TEXT("SavedTPSWork");
}


void ATPSworkGameMode::StartPlay()
{
    Super::StartPlay();

    if (GEngine)
    {
        // 显示调试信息五秒。 
        // -1"键"值（首个参数）说明我们无需更新或刷新此消息。
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Please shoot the target to get your score.");
    }
}


void ATPSworkGameMode::SaveGame(FString Name, int32 Score)
{
    UMySaveGame* SaveGameInstance=Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    if (SaveGameInstance)
    {
        if (SaveGameInstance->NameList.Contains(Name))
           
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Save Game failed. Your Name already exists");
            return;
        }
    }
    if (!SaveGameInstance)
    {
        SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    }
    int32 rank;
    for (rank = 0; rank < SaveGameInstance->ScoreList.Num(); rank++)
    {
        if (Score > SaveGameInstance->ScoreList[rank])
            break;
    }
    if (rank >= 10)
    {
        if (GEngine)
        {
            // 显示调试信息五秒。 
            // -1"键"值（首个参数）说明我们无需更新或刷新此消息。
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Only the top ten can save the score. You are rank " + FString::FromInt(rank + 1) + " .");
        }
        return;
    }
    else
    {
        // 即时保存游戏。
        SaveGameInstance->ScoreList.Insert(Score, rank);
        SaveGameInstance->NameList.Insert(Name, rank);
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
        {
            // 成功保存。
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Save Game Successfully. You are rank " + FString::FromInt(rank + 1) + " !");
        }
    }



}

void ATPSworkGameMode::LoadGame()
{

    if (UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
    {
        // 操作成功，LoadedGame现在将包含较早前保存的数据。
        
        GM_NameList = "";
        for (int32 i = 0; i < 10; i++)
        {
            if (i != 0)
                GM_NameList += " ";
            if(i<LoadedGame->NameList.Num())
                GM_NameList += LoadedGame->NameList[i];
            else
                GM_NameList += "--";
            
        }
        GM_ScoreList = "";
        for (int32 i = 0; i < 10; i++)
        {
            if (i != 0)
                GM_ScoreList += " ";
            if (i < LoadedGame->ScoreList.Num())
                GM_ScoreList += FString::FromInt(LoadedGame->ScoreList[i]);
            else
                GM_ScoreList += "--";
           
        }
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Load Saved Score Successfully.");
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Load Saved Score failed.");
    }
    
}
