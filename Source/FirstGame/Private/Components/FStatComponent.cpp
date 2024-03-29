// FStatComponent.cpp

#include "Components/FStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FGameInstance.h"

UFStatComponent::UFStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	// bWantsInitializeComponent: 'InitializeComponent() 함수 호출 유무'
	bWantsInitializeComponent = false;
}

void UFStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GameInstance = Cast<UFGameInstance>(GetWorld()->GetGameInstance());
	if (true == ::IsValid(GameInstance)) {
		// GameInstance에서 현재 스테이지 레벨 가져오기
		int32 CurrentStage = GameInstance->GetCurrentStage();
		// 'MaxHP, CurrentHP' 데이터를 담고 있는 DataTable 유효 확인
		if (nullptr != GameInstance->GetCharacterStatDataTable() ||
			nullptr != GameInstance->GetCharacterStatDataTableRow(CurrentStage)) {
			// DataTable에서 'CurrnetStage'번째 Row의 'MaxHP'값 가져오기
			float NewMaxHP = GameInstance->GetCharacterStatDataTableRow(CurrentStage)->MaxHP;
			SetMaxHP(NewMaxHP);
			SetCurrentHP(MaxHP);
		}
	}
}

void UFStatComponent::SetMaxHP(float InMaxHP)
{
	// 'MaxHP' 값이 바뀌었을 때
	if (true == OnMaxHPChangeDelegate.IsBound()) {
		// Broadcast(): 'OnMaxHPChangeDelegate'에 바인드되어 있는 콜백 함수들이 수행된다
		OnMaxHPChangeDelegate.Broadcast(MaxHP, InMaxHP);
	}

	// 'InMaxHP'의 값을 0 ~ 9999 범위로 자른다
	MaxHP = FMath::Clamp<float>(InMaxHP, 0.f, 9999);
}

void UFStatComponent::SetCurrentHP(float InCurrentHP)
{
	// 'CurrentHP' 값이 바뀌었을 때
	if (true == OnCurrentHPChangeDelegate.IsBound()) {
		OnCurrentHPChangeDelegate.Broadcast(CurrentHP, InCurrentHP);
	}

	// 'InCurrentHP'의 값을 0 ~ MaxHP 범위로 자른다
	CurrentHP = FMath::Clamp<float>(InCurrentHP, 0.f, MaxHP);

	// 'CurrentHP' 값이 0에 가까워졌을 때
	if (CurrentHP < KINDA_SMALL_NUMBER) {
		OnOutOfCurrentHPDelegate.Broadcast();
		CurrentHP = 0.f;
	}
}

