// FPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentStageChangedDelegate, int32, InOldCurrentStage, int32, InNewCurrentStage);

UCLASS()
class FIRSTGAME_API AFPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AFPlayerState();

	void InitPlayerState();

	// 'CurrentStage' 속성값 Getter, Setter
	int32 GetCurrentStage() const { return CurrentStage; }
	void SetCurrentStage(int32 InCurrentStage);

	// 'MaxStage' 속성값 Getter
	int32 GetMaxStage() const { return MaxStage; }

public:
	FOnCurrentStageChangedDelegate OnCurrentStageChangedDelegate;

private:
	// GameInstance 내부에 있는 StatComponent 정보(.csv 파일 내용)를 바인드할 속성
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AFPlayerState", meta = (AllowPrivateAccess))
	TObjectPtr<class UFGameInstance> FGI;

	// 현재 플레이하는 게임의 스테이지 단계를 저장하는 속성
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AFPlayerState", meta = (AllowPrivateAccess))
	int32 CurrentStage;

	// 게임의 스테이지 최대 단계를 저장하는 속성
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AFPlayerState", meta = (AllowPrivateAccess))
	int32 MaxStage;
};