// FMenuUI.cpp

#include "UI/FMenuUI.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Controllers/FPlayerController.h"

void UFMenuUI::NativeConstruct()
{
	// 버튼 클릭 시 멤버함수 호출 -- 버튼과 멤버함수 바인드
	ResumeGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnResumeGameButtonClicked);
	ReturnTitleButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnTitleButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
}

void UFMenuUI::OnResumeGameButtonClicked()
{
	// Player Controller 클래스에서 메뉴 토클 -> 메뉴 위젯 끄기
	AFPlayerController* PlayerController = Cast<AFPlayerController>(GetOwningPlayer());
	if (true == ::IsValid(PlayerController)) {
		PlayerController->ToggleMenu();
	}
}

void UFMenuUI::OnReturnTitleButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoadingStage")), true, FString(TEXT("NextLevel=TitleStage")));
}

void UFMenuUI::OnExitGameButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
