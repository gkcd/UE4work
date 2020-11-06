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
        // ��ʾ������Ϣ���롣 
        // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
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
            // ��ʾ������Ϣ���롣 
            // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Only the top ten can save the score. You are rank " + FString::FromInt(rank + 1) + " .");
        }
        return;
    }
    else
    {
        // ��ʱ������Ϸ��
        SaveGameInstance->ScoreList.Insert(Score, rank);
        SaveGameInstance->NameList.Insert(Name, rank);
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
        {
            // �ɹ����档
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Save Game Successfully. You are rank " + FString::FromInt(rank + 1) + " !");
        }
    }



}

void ATPSworkGameMode::LoadGame()
{

    if (UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
    {
        // �����ɹ���LoadedGame���ڽ���������ǰ��������ݡ�
        
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
