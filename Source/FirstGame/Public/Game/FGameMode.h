// FGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FGameMode.generated.h"

UCLASS()
class FIRSTGAME_API AFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFGameMode();

	// 로그인 후에 호출되는 함수(플레이어 컨트롤러, 폰... 준비 완료)
	// -- PlayerState 관리
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 'OnCurrentStageChangedDelegate' 델리게이트와 바운드할 함수
	UFUNCTION()
	void CurrentStageChanged(int32 NewCurrentStage);
};
