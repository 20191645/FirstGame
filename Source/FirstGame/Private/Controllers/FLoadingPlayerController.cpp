// FLoadingPlayerController.cpp

#include "Controllers/FLoadingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void AFLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (true == ::IsValid(GM)) {
		// 2.5초 딜레이 후 다음 레벨로 이동할 함수 호출
		FTimerHandle TimerHandle;
		float DelayTime = 2.5f;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
			OpenLevelFunc();
		}), DelayTime, false);
	}
}

void AFLoadingPlayerController::OpenLevelFunc()
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (true == ::IsValid(GM)) {
		// Options 매개변수에서 key값을 통해 value값 가져오기
		FString NextLevelString = UGameplayStatics::ParseOption(GM->OptionsString, FString(TEXT("NextLevel")));
		FString SavedTypeString = UGameplayStatics::ParseOption(GM->OptionsString, FString(TEXT("Saved")));
		UGameplayStatics::OpenLevel(GM, *NextLevelString, false, *SavedTypeString);
	}
}
