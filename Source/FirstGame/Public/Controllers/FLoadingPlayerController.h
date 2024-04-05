// FLoadingPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "Controllers/FUIPlayerController.h"
#include "FLoadingPlayerController.generated.h"

UCLASS()
class FIRSTGAME_API AFLoadingPlayerController : public AFUIPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	// 딜레이 적용 후 실행될 함수 -- OpenLevel()
	void OpenLevelFunc();
};
