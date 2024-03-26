// AN_CheckHit.cpp

#include "Animations/AnimNotifies/AN_CheckHit.h"
#include "Characters/FRPGCharacter.h"
#include "Characters/FNPCharacter.h"

void UAN_CheckHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (true == ::IsValid(MeshComp)) {
		// 공격 주체가 FRPGCharacter인 경우
		if (MeshComp->GetOwner()->IsA(AFRPGCharacter::StaticClass())) {
			AFRPGCharacter* AttackingCharacter = Cast<AFRPGCharacter>(MeshComp->GetOwner());
			if (true == ::IsValid(AttackingCharacter)) {
				AttackingCharacter->CheckHit();
			}
		}
		// 공격 주체가 FNPCharacter인 경우
		else if (MeshComp->GetOwner()->IsA(AFNPCharacter::StaticClass())) {
			AFNPCharacter* AttackingCharacter = Cast<AFNPCharacter>(MeshComp->GetOwner());
			if (true == ::IsValid(AttackingCharacter)) {
				AttackingCharacter->CheckHit();
			}
		}
	}
}