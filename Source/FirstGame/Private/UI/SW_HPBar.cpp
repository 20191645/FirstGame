// SW_HPBar.cpp

#include "UI/SW_HPBar.h"
#include "Components/ProgressBar.h"
#include "Characters/FCharacter.h"
#include "Components/FStatComponent.h"

void USW_HPBar::SetMaxHP(float InMaxHP)
{
	// SetMaxFigure(): SW_Bar 클래스에서 상속받은 멤버함수
	SetMaxFigure(InMaxHP);
}

void USW_HPBar::InitializeHPBarWidget(UFStatComponent* NewStatComponent)
{
	// 새로운 StatComponent의 'CurrentHP' 값으로 초기화
	OnCurrentHPChange(0, NewStatComponent->GetCurrentHP());
}

void USW_HPBar::OnMaxHPChange(float InOldMaxHP, float InNewMaxHP)
{
	SetMaxFigure(InNewMaxHP);
	OnCurrentHPChange(0, InNewMaxHP);
}

void USW_HPBar::OnCurrentHPChange(float InOldHP, float InNewHP)
{
	if (true == ::IsValid(Bar)) {
		if (KINDA_SMALL_NUMBER < MaxFigure) {
			Bar->SetPercent(InNewHP / MaxFigure);
		}
		else {
			Bar->SetPercent(0.f);
		}
	}
}

void USW_HPBar::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯을 소유한 액터에게 위젯 정보를 알려준다
	AFCharacter* OwningCharacter = Cast<AFCharacter>(OwningActor);
	if (true == ::IsValid(OwningCharacter)) {
		OwningCharacter->SetWidget(this);
	}
}