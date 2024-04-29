// SW_MPBar.cpp

#include "UI/SW_MPBar.h"
#include "Components/ProgressBar.h"
#include "Characters/FCharacter.h"
#include "Components/FStatComponent.h"

void USW_MPBar::SetMaxMP(float InMaxMP)
{
	// SetMaxFigure(): SW_Bar 클래스에서 상속받은 멤버함수
	SetMaxFigure(InMaxMP);
}

void USW_MPBar::InitializeMPBarWidget(UFStatComponent* NewStatComponent)
{
	// 새로운 StatComponent의 'CurrentMP' 값으로 초기화
	OnCurrentMPChange(0, NewStatComponent->GetCurrentMP());
}

void USW_MPBar::OnMaxMPChange(float InOldMaxMP, float InNewMaxMP)
{
	SetMaxFigure(InNewMaxMP);
	OnCurrentMPChange(0, InNewMaxMP);
}

void USW_MPBar::OnCurrentMPChange(float InOldMP, float InNewMP)
{
	if (true == ::IsValid(Bar)) {
		if (KINDA_SMALL_NUMBER < MaxFigure) {
			Bar->SetPercent(InNewMP / MaxFigure);
		}
		else {
			Bar->SetPercent(0.f);
		}
	}
}

void USW_MPBar::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯을 소유한 액터에게 위젯 정보를 알려준다
	AFCharacter* OwningCharacter = Cast<AFCharacter>(OwningActor);
	if (true == ::IsValid(OwningCharacter)) {
		OwningCharacter->SetWidget(this);
	}
}
