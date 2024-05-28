// BTTask_Skill.h

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Skill.generated.h"

UCLASS()
class FIRSTGAME_API UBTTask_Skill : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Skill();
	
protected:
	// 매 Tick마다 SNPCharacter 클래스의 'bIsAttacking' 속성값 검사
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Task 실행하면 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
