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
        // 显示调试信息五秒。 
        // -1"键"值（首个参数）说明我们无需更新或刷新此消息。
        GEngine->AddOnScreenDebugMessage(-1, 999999.0f, FColor::Red, "Please shoot the target to get your score.");
    }
}
