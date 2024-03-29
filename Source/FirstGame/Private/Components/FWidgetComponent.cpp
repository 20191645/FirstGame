// FWidgetComponent.cpp

#include "Components/FWidgetComponent.h"
#include "UI/FirstUserWidget.h"

void UFWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 위젯의 소유 액터 지정
	UFirstUserWidget* FWidget = Cast<UFirstUserWidget>(GetWidget());
	if (true == ::IsValid(FWidget)) {
		FWidget->SetOwningActor(GetOwner());
	}
}
