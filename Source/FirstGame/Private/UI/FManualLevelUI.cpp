// FManualLevelUI.cpp

#include "UI/FManualLevelUI.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UFManualLevelUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트와 멤버함수 바인드
	NextButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnNextButtonClicked);
}

void UFManualLevelUI::OnNextButtonClicked()
{
	// 다음 레벨로 이동
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LoadingStage"), true, FString(TEXT("NextLevel=FirstStage?Saved=true")));
}
