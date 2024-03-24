// FAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FAIController.generated.h"

UCLASS()
class FIRSTGAME_API AFAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFAIController();
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnPatrolTimerElapsed();

public:
	// 타이머 핸들
	FTimerHandle PatrolTimerHandle = FTimerHandle();
	// 타이머가 울리는 주기
	static const float PatrolRepeatInterval;
	// 이동 범위
	static const float PatrolRadius;
};
