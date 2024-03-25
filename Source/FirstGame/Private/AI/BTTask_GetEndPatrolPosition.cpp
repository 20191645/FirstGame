// BTTask_GetEndPatrolPosition.cpp

#include "AI/BTTask_GetEndPatrolPosition.h"
#include "Controllers/FAIController.h"
#include "Characters/FNPCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_GetEndPatrolPosition::UBTTask_GetEndPatrolPosition()
{
	// Behavior Tree에서 보이는 노드 이름 설정
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTTask_GetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (EBTNodeResult::Failed == Result) {
		return Result;
	}

	// AIController Valid 체크
	AFAIController* AIController = Cast<AFAIController>(OwnerComp.GetAIOwner());
	if (false == ::IsValid(AIController)) {
		return Result = EBTNodeResult::Failed;
	}

	// NPCharacter Valid 체크
	AFNPCharacter* NPC = Cast<AFNPCharacter>(AIController->GetPawn());
	if (false == ::IsValid(NPC)) {
		return Result = EBTNodeResult::Failed;
	}

	// NavigationSystem Valid 체크
	UNavigationSystemV1* NS = UNavigationSystemV1::GetNavigationSystem(NPC->GetWorld());
	if (false == ::IsValid(NS)) {
		return Result = EBTNodeResult::Failed;
	}

	// StartPatrolPosition: Blackboard의 Key('StartPatrolPosition') 값을 가져온다
	FVector StartPatrolPositiion =
		OwnerComp.GetBlackboardComponent()->GetValueAsVector(AFAIController::StartPatrolPositionKey);

	// GetRandomPointInNavigableRadius: 이동 가능한 랜덤 목적지를 'EndPatrolLocation'에 반환
	FNavLocation EndPatrolLocation;
	if (true == NS->GetRandomPointInNavigableRadius(StartPatrolPositiion, AIController->PatrolRadius, EndPatrolLocation)) {
		// 'EndPatrolLocation'의 값을 'AFAIController' 클래스의 'EndPatrolPositionKey'에 저장된 이름의 Key에 저장
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AFAIController::EndPatrolPositionKey, EndPatrolLocation.Location);
		return Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
