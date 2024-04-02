// FLoadingPlayerController.cpp

#include "Controllers/FLoadingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void AFLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (true == ::IsValid(GM)) {
		// 다음 레벨로 이동
		UGameplayStatics::OpenLevel(GM, TEXT("FirstStage"));
	}
}
