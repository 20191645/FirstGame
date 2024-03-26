// FAIController.cpp

#include "Controllers/FAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const float AFAIController::PatrolRadius(500.f);
const FName AFAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName AFAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName AFAIController::TargetActorKey(TEXT("TargetActor"));

AFAIController::AFAIController()
{
    Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
    // BrainComponent: AIController 클래스에 정의된 속성, 
    // UBrainComponent(부모) <- UBehaviorTreeComponent(자식)
    BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
}

void AFAIController::BeginAI(APawn* InPawn)
{
    UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
    if (true == ::IsValid(BlackboardComponent)) {
        // 'BlackboardDataAsset'을 Blackboard로 사용
        if (true == UseBlackboard(BlackboardDataAsset, BlackboardComponent)) {
            // BehaviorTree 실행 및 확인
            bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
            ensure(true == bRunSucceeded);
        
            // 'StartPatrolPositionKey'에 저장된 Key 이름을 통해 'StartPatrolPosition'에 현재 액터의 위치 벡터값 저장
            BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
        }
    }
}

void AFAIController::EndAI()
{
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (true == ::IsValid(BehaviorTreeComponent)) {
        // BehaviorTree 중단
        BehaviorTreeComponent->StopTree();
    }
}

void AFAIController::BeginPlay()
{
	Super::BeginPlay();

    // 현재 빙의하고 있는 폰 가져오기
    APawn* ControlledPawn = GetPawn();
    if (true == ::IsValid(ControlledPawn)) {
        BeginAI(ControlledPawn);
    }
}

void AFAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

    EndAI();
}