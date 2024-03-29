// SW_Bar.cpp

#include "UI/SW_Bar.h"
#include "Components/ProgressBar.h"
#include "Characters/FNPCharacter.h"
#include "Components/FStatComponent.h"

USW_Bar::USW_Bar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) // 반드시 필요한 코드, 위젯 구현할 때 꼭 넣어야 한다
{
}

void USW_Bar::SetMaxFigure(float InMaxFigure)
{
	// Bar 속성 최대값이 0에 가까울 때
	if (InMaxFigure < KINDA_SMALL_NUMBER) {
		MaxFigure = 0.f;
		return;
	}

	MaxFigure = InMaxFigure;
}

void USW_Bar::NativeConstruct()
{
	Super::NativeConstruct();

	// 하드코딩으로 위젯과 속성을 바인드 -- 'BarWidget'이란 이름의 ProgressBar 가져오기
	Bar = Cast<UProgressBar>(GetWidgetFromName("BarWidget"));
	check(nullptr != Bar);
}
