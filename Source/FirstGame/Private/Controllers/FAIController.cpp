// FAIController.cpp

#include "Controllers/FAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

const float AFAIController::PatrolRepeatInterval(3.f);
const float AFAIController::PatrolRadius(500.f);

AFAIController::AFAIController()
{
}

void AFAIController::BeginPlay()
{
	Super::BeginPlay();

    // 타이머의 타이머 핸들에 'PatrolTimerHandle'을 채워넣고, Timer가 'PatrolRepeatInterval' 주기로 울리면 
    // 'OnPatrolTimerElapsed()' 멤버함수가 호출된다
	GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &ThisClass::OnPatrolTimerElapsed, PatrolRepeatInterval, true);
}

void AFAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

    // 설정해놨던 타이머를 Clear 해준다
	GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
}

void AFAIController::OnPatrolTimerElapsed()
{
    // 현재 빙의하고 있는 폰 가져오기
    APawn* ControlledPawn = GetPawn();
    if (true == ::IsValid(ControlledPawn))
    {
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
        if (true == ::IsValid(NavSystem))
        {
            FNavLocation NextLocation;
            if (true == NavSystem->GetRandomPointInNavigableRadius(ControlledPawn->GetActorLocation(), PatrolRadius, NextLocation))
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
            }
        }
    }
}
