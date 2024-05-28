// AN_CheckHit_Skill.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_CheckHit_Skill.generated.h"

UCLASS()
class FIRSTGAME_API UAN_CheckHit_Skill : public UAnimNotify
{
	GENERATED_BODY()

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
