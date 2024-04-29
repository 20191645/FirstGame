// FStatComponent.cpp

#include "Components/FStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FGameInstance.h"
#include "Characters/FRPGCharacter.h"
#include "Game/FPlayerState.h"

UFStatComponent::UFStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	// bWantsInitializeComponent: 'InitializeComponent() 함수 호출 유무'
	bWantsInitializeComponent = false;
}

void UFStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// StatComponent를 소유한 캐릭터 불러오기
	AFCharacter* OwningPlayerCharacter = Cast<AFCharacter>(GetOwner());
	int32 CurrentStage = 1;
	if (true == ::IsValid(OwningPlayerCharacter)) {
		AFPlayerState* PS = Cast<AFPlayerState>(OwningPlayerCharacter->GetPlayerState());
		if (true == ::IsValid(PS)) {
			// 현재 CurrentStage 가져오기
			CurrentStage = PS->GetCurrentStage();

			if (false == PS->OnCurrentStageChangedDelegate.IsAlreadyBound(this, &ThisClass::OnCurrentStageChanged)) {
				// 'OnCurrentStageChanged' 델리게이트에 OnCurrentStageChanged() 멤버 함수 바인드
				PS->OnCurrentStageChangedDelegate.AddDynamic(this, &ThisClass::OnCurrentStageChanged);
			}
		}
	}

	GameInstance = Cast<UFGameInstance>(GetWorld()->GetGameInstance());
	if (true == ::IsValid(GameInstance)) {
		// 'HP, MP' 데이터를 담고 있는 DataTable 유효 확인
		if (nullptr != GameInstance->GetCharacterStatDataTable() ||
			nullptr != GameInstance->GetCharacterStatDataTableRow(CurrentStage)) {
			// DataTable에서 'CurrnetStage'번째 Row의 'MaxHP'값 가져오기
			float NewMaxHP = GameInstance->GetCharacterStatDataTableRow(CurrentStage)->MaxHP;
			SetMaxHP(NewMaxHP);
			SetCurrentHP(MaxHP);

			// DataTable에서 'CurrnetStage'번째 Row의 'MaxMP'값 가져오기
			float NewMaxMP = GameInstance->GetCharacterStatDataTableRow(CurrentStage)->MaxMP;
			SetMaxMP(NewMaxMP);
			SetCurrentMP(MaxMP);
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

	// 'InMaxHP'의 값을 0 ~ 999 범위로 자른다
	MaxHP = FMath::Clamp<float>(InMaxHP, 0.f, 1000);
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

void UFStatComponent::SetMaxMP(float InMaxMP)
{
	// 'MaxMP' 값이 바뀌었을 때
	if (true == OnMaxMPChangeDelegate.IsBound()) {
		// Broadcast(): 'OnMaxMPChangeDelegate'에 바인드되어 있는 콜백 함수들이 수행된다
		OnMaxMPChangeDelegate.Broadcast(MaxMP, InMaxMP);
	}

	// 'InMaxMP'의 값을 0 ~ 999 범위로 자른다
	MaxMP = FMath::Clamp<float>(InMaxMP, 0.f, 2000);
}

void UFStatComponent::SetCurrentMP(float InCurrentMP)
{
	// 'CurrentHP' 값이 바뀌었을 때
	if (true == OnCurrentMPChangeDelegate.IsBound()) {
		OnCurrentMPChangeDelegate.Broadcast(CurrentMP, InCurrentMP);
	}

	// 'InCurrentMP'의 값을 0 ~ MaxHP 범위로 자른다
	CurrentMP = FMath::Clamp<float>(InCurrentMP, 0.f, MaxMP);

	// 'CurrentMP' 값이 0에 가까워졌을 때
	if (CurrentMP < KINDA_SMALL_NUMBER) {
		CurrentMP = 0.f;
	}
}

void UFStatComponent::OnCurrentStageChanged(int32 NewCurrentStage)
{
	// 스테이지 단계 변화 시 HP 업데이트
	SetMaxHP(GameInstance->GetCharacterStatDataTableRow(NewCurrentStage)->MaxHP);
	SetCurrentHP(GameInstance->GetCharacterStatDataTableRow(NewCurrentStage)->MaxHP);

	// 스테이지 단계 변화 시 MP 업데이트
	SetMaxMP(GameInstance->GetCharacterStatDataTableRow(NewCurrentStage)->MaxMP);
	SetCurrentMP(GameInstance->GetCharacterStatDataTableRow(NewCurrentStage)->MaxMP);
}