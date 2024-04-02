// FUIPlayerController.cpp

#include "Controllers/FUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AFUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (true == ::IsValid(UIWidgetClass)) {
		// CreateWidget()이 호출될 때 UIWidgetInstance->NativeOnInitialize() 함수가 호출된다
		// 위젯 객체 생성
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		if (true == ::IsValid(UIWidgetInstance)) {
			// AddToViewPort()가 호출될 때 UIWidgetInstance->NativeConstruct() 함수가 호출된다
			UIWidgetInstance->AddToViewport();

			// 게임 플레이하지 않기 때문에 UI에 포커싱
			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);

			// 마우스 커서를 보이게 한다
			bShowMouseCursor = true;
		}
	}
}
