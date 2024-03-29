// FGameInstance.cpp

#include "Game/FGameInstance.h"

void UFGameInstance::Init()
{
	Super::Init();

	// 데이터 테이블이 유효하고 Row가 1줄 이상인지 확인
	if (false == ::IsValid(CharacterStatDataTable) ||
		CharacterStatDataTable->GetRowMap().Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("Not enough data in CharacterStatDataTable."));
	}
	else {
		// Row 개수만큼 순회하면서 각 Row 내용을 체크
		for (int32 i = 1; i <= CharacterStatDataTable->GetRowMap().Num(); ++i) {
			check(nullptr != GetCharacterStatDataTableRow(i));
		}
	}

	// 게임 시작 시 스테이지 단계 초기화
	CurrentStage = 1;
}

void UFGameInstance::Shutdown()
{
	Super::Shutdown();
}

FFStatTableRow* UFGameInstance::GetCharacterStatDataTableRow(int32 InStage)
{
	if (true == ::IsValid(CharacterStatDataTable)) {
		return CharacterStatDataTable->FindRow<FFStatTableRow>(*FString::FromInt(InStage), TEXT(""));
	}

	return nullptr;
}
