// FBuffComponent.cpp

#include "Components/FBuffComponent.h"
#include "Characters/FRPGCharacter.h"

UFBuffComponent::UFBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	// bWantsInitializeComponent: 'InitializeComponent() 함수 호출 유무'
	bWantsInitializeComponent = false;

	// 속성 초기화
	Duration = 3.0f;
	MaxStack = 5;

	SetCurrentStack(0);
	SetBuffName("Debuff");
}

void UFBuffComponent::SetCurrentStack(int32 InCurrentStack)
{
	// 'InCurrentStack'의 값을 0 ~ MaxStack 범위로 자른다
	CurrentStack = FMath::Clamp<float>(InCurrentStack, 0, MaxStack);

	// 'CurrentStack' 값이 바뀌었을 때
	if (true == OnCurrentStackChangeDelegate.IsBound()) {
		// Broadcast(): 'OnCurrentStackChangeDelegate'에 바인드되어 있는 콜백 함수들이 수행된다
		OnCurrentStackChangeDelegate.Broadcast(CurrentStack);
	}
}