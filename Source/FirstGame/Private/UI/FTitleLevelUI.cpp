// FTitleLevelUI.cpp

#include "UI/FTitleLevelUI.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UFTitleLevelUI::UFTitleLevelUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFTitleLevelUI::NativeConstruct()
{
	// 버튼 클릭 시 멤버함수 호출 -- 버튼과 멤버함수 바인드
	NewGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
}

void UFTitleLevelUI::OnNewGameButtonClicked()
{
	// 다음 레벨로 이동
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LoadingStage"));
}

void UFTitleLevelUI::OnExitGameButtonClicked()
{
	// 게임 종료시키는 함수
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
