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

private:
	// Blackboard 데이터 정보를 담을 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AFAIController", meta = (AllowPrivateAccess))
	TObjectPtr<class UBlackboardData> BlackboardDataAsset;
	// BehaviorTree 데이터 정보를 담을 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AFAIController", meta = (AllowPrivateAccess))
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};
