// FLobbyLevelUI.cpp

#include "UI/FLobbyLevelUI.h"
#include "Characters/FCharacter.h"
#include "Components/Button.h"
#include "Game/FGameInstance.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"

void UFLobbyLevelUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트와 멤버함수 바인드
	SubmitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSubmitButtonClicked);
}

void UFLobbyLevelUI::OnSubmitButtonClicked()
{
	// 플레이어 이름을 1 ~ 9글자 사이로 입력받는다
	// FString PlayerName = EditPlayerName->GetText().ToString();
	PlayerName = EditPlayerName->GetText().ToString();
	if (PlayerName.Len() <= 0 || 10 <= PlayerName.Len()) {
		return;
	}

	SaveInitializedSaveData();
	// 다음 레벨로 이동
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ManualStage"));
}

void UFLobbyLevelUI::SaveInitializedSaveData()
{
	UFPlayerStateSave* PlayerStateSave = NewObject<UFPlayerStateSave>();
	PlayerStateSave->PlayerName = PlayerName;
	PlayerStateSave->CurrentStage = 1;

	// Save Game 파일 저장
	UGameplayStatics::SaveGameToSlot(PlayerStateSave, TEXT("PlayerState"), 0);
}
