// FLoadingPlayerController.cpp

#include "Controllers/FLoadingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void AFLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*
	// 로딩 화면 딜레이를 위해 해당 코드를 블루프린트로 구현
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (true == ::IsValid(GM)) {
		// 다음 레벨로 이동
		UGameplayStatics::OpenLevel(GM, TEXT("FirstStage"));
	}
	*/
}