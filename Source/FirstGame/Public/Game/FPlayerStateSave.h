// FPlayerStateSave.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPlayerStateSave.generated.h"

UCLASS()
class FIRSTGAME_API UFPlayerStateSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UFPlayerStateSave();

	// 플레이어 이름
	UPROPERTY()
	FString PlayerName;

	// 현재 게임의 스테이지 단계
	UPROPERTY()
	int32 CurrentStage;
};
