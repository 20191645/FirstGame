// BTDecorator_IsInAttackRange.h

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"

UCLASS()
class FIRSTGAME_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInAttackRange();

protected:
	// Decorator 클래스에서 상속받는 함수 [const 선언] -> Decorator 클래스 속성값 변경X
	// -- 원하는 조건이 달성되었는지 파악
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	// 공격 범위
	static const float AttackRange;
};
