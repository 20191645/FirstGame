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
	
	void BeginAI(APawn* InPawn);
	void EndAI();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// 이동 범위 -- static 타입이기 때문에 .cpp 파일 상위에서 초기화
	static const float PatrolRadius;

	// Blackboard(BB_NPC)의 Key 이름 저장할 속성 -- static 타입이기 때문에 .cpp 파일 상위에서 초기화
	static const FName StartPatrolPositionKey;
	static const FName EndPatrolPositionKey;

private:
	// Blackboard 데이터 정보를 담을 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AFAIController", meta = (AllowPrivateAccess))
	TObjectPtr<class UBlackboardData> BlackboardDataAsset;
	// BehaviorTree 데이터 정보를 담을 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AFAIController", meta = (AllowPrivateAccess))
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};
