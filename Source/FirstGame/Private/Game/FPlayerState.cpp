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

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (true == ::IsValid(GM)) {
		FString SavedTypeString = UGameplayStatics::ParseOption(GM->OptionsString, FString(TEXT("Saved")));
		// 저장된 게임 파일을 사용한다
		if (true == SavedTypeString.Equals("true")) {
			// 'SaveSlotName' 이름의 슬롯으로부터 'SaveGame'데이터[PlayerStateSave] 불러오기
			UFPlayerStateSave* PlayerStateSave = Cast<UFPlayerStateSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
			if (false == ::IsValid(PlayerStateSave)) {
				// GetMutableDefault(): 수정 가능한 CDO 개체 불러오기
				PlayerStateSave = GetMutableDefault<UFPlayerStateSave>();
			}

			// 'SaveGame'데이터[PlayerStateSave]에서 읽어온 값으로 초기값 세팅
			SetPlayerName(PlayerStateSave->PlayerName);
			SetCurrentStage(PlayerStateSave->CurrentStage);
		}
		// 저장된 게임 파일을 사용하지 않는 새 게임 시작
		else {
			SetPlayerName(TEXT("Player0"));
			SetCurrentStage(1);
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
	if (true == UGameplayStatics::SaveGameToSlot(PlayerStateSave, SaveSlotName, 0)) {
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Saved.")));
	}
}
