// FNPCharacter.cpp

#include "Characters/FNPCharacter.h"
#include "Controllers/FAIController.h"

AFNPCharacter::AFNPCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // 레벨에 새롭게 배치되거나 생성되면 FAIController 빙의가 자동으로 이뤄진다
    AIControllerClass = AFAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
