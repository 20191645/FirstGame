// AN_CheckHit_Skill.cpp

#include "Animations/AnimNotifies/AN_CheckHit_Skill.h"
#include "Characters/FNPCharacter.h"

void UAN_CheckHit_Skill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (true == ::IsValid(MeshComp)) {
		// 공격 주체가 FNPCharacter인 경우
		if (MeshComp->GetOwner()->IsA(AFNPCharacter::StaticClass())) {
			AFNPCharacter* AttackingCharacter = Cast<AFNPCharacter>(MeshComp->GetOwner());
			if (true == ::IsValid(AttackingCharacter)) {
				AttackingCharacter->CheckHit_Skill();
			}
		}
	}
}
