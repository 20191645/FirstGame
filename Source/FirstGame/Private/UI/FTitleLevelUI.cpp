// FTitleLevelUI.cpp

#include "UI/FTitleLevelUI.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FPlayerState.h"
#include "Game/FPlayerStateSave.h"

UFTitleLevelUI::UFTitleLevelUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFTitleLevelUI::NativeConstruct()
{
	// 버튼 클릭 시 멤버함수 호출 -- 버튼과 멤버함수 바인드
	NewGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
	SavedGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSavedGameButtonClicked);

	// 처음에는 버튼 비활성화 - 'SaveGame' 데이터 파일이 로드되면 활성화
	SavedGameButton->SetIsEnabled(false);
	const AFPlayerState* PS = GetDefault<AFPlayerState>();
	if (true == ::IsValid(PS)) {
		UFPlayerStateSave* PlayerStateSave = Cast<UFPlayerStateSave>(UGameplayStatics::LoadGameFromSlot(PS->SaveSlotName, 0));
		if (true == ::IsValid(PlayerStateSave)) {
			SavedGameButton->SetIsEnabled(true);
		}
	}
}

void UFTitleLevelUI::OnNewGameButtonClicked()
{
	// 다음 레벨로 이동
	// OpenLevel() 함수의 Options 매개변수: FString(TEXT("NextLevel=FirstStage?Saved=false"))
	// => 'NextLevel'이 Key, 'LobbyStage'이 Value 
	// -- Loading 레벨에서는 'NextLevel'을 파싱해서 'LobbyStage' 값을 얻어낸다
	// NextLevel: 이동할 레벨 이름
	// Saved: 저장된 게임 파일 쓸지 선택
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoadingStage")), true, FString(TEXT("NextLevel=LobbyStage?Saved=false")));

}

void UFTitleLevelUI::OnExitGameButtonClicked()
{
	// 게임 종료시키는 함수
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UFTitleLevelUI::OnSavedGameButtonClicked()
{
	// 다음 레벨로 이동
	// 'NextLevel'이 Key, 'FirstStage'이 Value 
	// -- Loading 레벨에서는 'NextLevel'을 파싱해서 'FirstStage' 값을 얻어낸다
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoadingStage")), true, FString(TEXT("NextLevel=FirstStage?Saved=true")));
}
