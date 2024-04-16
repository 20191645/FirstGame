// FPlayerState.cpp

#include "Game/FPlayerState.h"
#include "Game/FGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FPlayerStateSave.h"
#include "GameFramework/GameModeBase.h"

FString AFPlayerState::SaveSlotName(TEXT("PlayerState"));

AFPlayerState::AFPlayerState()
{
}

void AFPlayerState::InitPlayerState()
{
	UE_LOG(LogTemp, Log, TEXT("AFPlayerState::InitPlayerState()"));

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

	// 'SaveGame' 파일을 통해 저장된 플레이어 정보 가져와서 초기화
	UFPlayerStateSave* PlayerStateSave = Cast<UFPlayerStateSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (false == ::IsValid(PlayerStateSave)) {
		// 저장된 'SaveGame' 파일이 없으면 디폴트 값 가져오기
		PlayerStateSave = GetMutableDefault<UFPlayerStateSave>();
	}
	SetPlayerName(PlayerStateSave->PlayerName);
	SetCurrentStage(PlayerStateSave->CurrentStage);

	// 플레이어 정보가 바뀌었으므로 저장
	SavePlayerState();
}

void AFPlayerState::SetCurrentStage(int32 InCurrentStage)
{
	UE_LOG(LogTemp, Log, TEXT("AFPlayerState::SetCurrentStage()"));

	// 'InCurrentStage' 값을 1 ~ MaxStage 범위로 자르기
	int32 ActualStage = FMath::Clamp<int32>(InCurrentStage, 1, MaxStage);

	FFStatTableRow* RowData = FGI->GetCharacterStatDataTableRow(ActualStage);
	if (nullptr != RowData) {
		CurrentStage = ActualStage;
		// 'CurrentStage' 값 변화가 생겼기 때문에 델리게이트에 연결된 함수들에게 broadcast
		OnCurrentStageChangedDelegate.Broadcast(CurrentStage);
	}

	// CurrentStage 값에 변화가 생길때마다 PlayerState에 저장하는 함수 호출
	SavePlayerState();
}

void AFPlayerState::SavePlayerState()
{
	// 'FPlayerStateState' 클래스 개체 생성 후 값 저장
	UFPlayerStateSave* PlayerStateSave = NewObject<UFPlayerStateSave>();
	PlayerStateSave->PlayerName = GetPlayerName();
	PlayerStateSave->CurrentStage = GetCurrentStage();

	// 'SaveGame'데이터[PlayerStateSave]를 'SaveSlotName' 이름의 슬롯에 저장
	// -> 파일에 저장되기 때문에 게임을 종료해도 다시 플레이하면 속성 값을 불러와 유지한다
	UGameplayStatics::SaveGameToSlot(PlayerStateSave, SaveSlotName, 0);
}
