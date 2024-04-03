// FPlayerState.cpp

#include "Game/FPlayerState.h"
#include "Game/FGameInstance.h"
#include "Kismet/GameplayStatics.h"

AFPlayerState::AFPlayerState()
{
}

void AFPlayerState::InitPlayerState()
{
	FGI = Cast<UFGameInstance>(GetWorld()->GetGameInstance());
	if (true == ::IsValid(FGI)) {
		// Stat Data Table 유효 검사
		if (nullptr != FGI->GetCharacterStatDataTable() ||
			nullptr != FGI->GetCharacterStatDataTableRow(1)) {
			// 스테이지 단계 1로 초기화
			MaxStage = FGI->GetCharacterStatDataTable()->GetRowMap().Num();
			CurrentStage = 1;
		}
	}
}

void AFPlayerState::SetCurrentStage(int32 InCurrentStage)
{
	int32 OldCurrentStage = CurrentStage;
	// 'InCurrentStage' 값을 1 ~ MaxStage 범위로 자르기
	int32 ActualStage = FMath::Clamp<int32>(InCurrentStage, 1, MaxStage);

	FFStatTableRow* RowData = FGI->GetCharacterStatDataTableRow(ActualStage);
	if (nullptr != RowData) {
		CurrentStage = ActualStage;
		// 'CurrentStage' 값 변화가 생겼기 때문에 델리게이트에 연결된 함수들에게 broadcast
		OnCurrentStageChangedDelegate.Broadcast(OldCurrentStage, CurrentStage);
	}
}