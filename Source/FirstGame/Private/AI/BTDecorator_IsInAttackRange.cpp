// BTDecorator_IsInAttackRange.cpp

#include "AI/BTDecorator_IsInAttackRange.h"
#include "Controllers/FAIController.h"
#include "Characters/FNPCharacter.h"
#include "Characters/FCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

const float UBTDecorator_IsInAttackRange::AttackRange(100.f);

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	// Behavior Tree에서 보이는 노드 이름 설정
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AFAIController* AIC = Cast<AFAIController>(OwnerComp.GetAIOwner());
	if (true == ::IsValid(AIC)) {
		AFNPCharacter* NPC = Cast<AFNPCharacter>(AIC->GetPawn());
		if (true == ::IsValid(NPC)) {
			// 'BTService_DetectPlayerCharacter'에서 감지된 값(=TargetActorKey) 가져오기
			AFCharacter* TargetPlayerCharacter = Cast<AFCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AFAIController::TargetActorKey));
			if (true == ::IsValid(TargetPlayerCharacter)) {
				// 타겟 캐릭터와의 거리가 Attack Range 범위 내이면 true
				return bResult = (NPC->GetDistanceTo(TargetPlayerCharacter) <= AttackRange);
			}
		}
	}

	return bResult = false;
}
