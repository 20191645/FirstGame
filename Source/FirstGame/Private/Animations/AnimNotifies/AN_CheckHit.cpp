// AN_CheckHit.cpp

#include "Animations/AnimNotifies/AN_CheckHit.h"
#include "Characters/FRPGCharacter.h"

void UAN_CheckHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (true == ::IsValid(MeshComp)) {
		AFRPGCharacter* AttackingCharacter = Cast<AFRPGCharacter>(MeshComp->GetOwner());
		if (true == ::IsValid(AttackingCharacter)) {
			AttackingCharacter->CheckHit();
		}
	}
}