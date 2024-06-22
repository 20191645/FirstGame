// BTTask_Skill.cpp

#include "AI/BTTask_Skill.h"
#include "Controllers/FAIController.h"
#include "Characters/FNPCharacter.h"

UBTTask_Skill::UBTTask_Skill()
{
	// TickTask()가 매 Tick마다 실행될 수 있도록 설정
	bNotifyTick = true;
}

void UBTTask_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AFAIController* AIC = Cast<AFAIController>(OwnerComp.GetAIOwner());
	if (true == ::IsValid(AIC)) {
		AFNPCharacter* NPC = Cast<AFNPCharacter>(AIC->GetPawn());
		if (true == ::IsValid(NPC)) {
			// FNPCharacter 클래스의 'bIsUsingSkill' 속성값 확인
			if (false == NPC->IsNowUsingSkill()) {
				// FinishLatentTask(, Succeeded): 스킬 애니메이션(Animation Montage)이 끝났음을 알려준다
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}

EBTNodeResult::Type UBTTask_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AFAIController* AIC = Cast<AFAIController>(OwnerComp.GetAIOwner());
	if (true == ::IsValid(AIC)) {
		AFNPCharacter* NPC = Cast<AFNPCharacter>(AIC->GetPawn());
		if (true == ::IsValid(NPC)) {
			// FNPCharacter 클래스의 SKill() 멤버 함수 호출
			NPC->Skill();
		}
	}

	// 아직 스킬 애니메이션이 실행중이므로 InProgress 반환
	return EBTNodeResult::InProgress;
}
