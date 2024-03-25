// BTService_DetectPlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectPlayerCharacter.generated.h"

UCLASS()
class FIRSTGAME_API UBTService_DetectPlayerCharacter : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_DetectPlayerCharacter();

protected:
	// Service가 부착된 Composite Task 활성화 시 호출
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
