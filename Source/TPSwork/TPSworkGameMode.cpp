// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSworkGameMode.h"
#include "TPSworkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSworkGameMode::ATPSworkGameMode()
{
    // set default pawn class to our Blueprinted character
  //  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_ThirdPersonCharacter"));
  //  if (PlayerPawnBPClass.Class != NULL)
  //  {
   //     DefaultPawnClass = PlayerPawnBPClass.Class;
   // }
}


void ATPSworkGameMode::StartPlay()
{
    Super::StartPlay();

    if (GEngine)
    {
        // ��ʾ������Ϣ���롣 
        // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
        GEngine->AddOnScreenDebugMessage(-1, 999999.0f, FColor::Red, "Please shoot the target to get your score.");
    }
}
