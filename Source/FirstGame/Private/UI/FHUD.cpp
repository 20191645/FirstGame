// FHUD.cpp

#include "UI/FHUD.h"
#include "Components/FStatComponent.h"
#include "UI/SW_HPBar.h"
#include "UI/SW_Bar.h"
#include "Components/TextBlock.h"
#include "Game/FGameInstance.h"
#include "Game/FPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/FBuffComponent.h"
#include "UI/SW_MPBar.h"

void UFHUD::BindStatComponent(UFStatComponent* InStatComponent)
{
	if (true == ::IsValid(InStatComponent)) {
		StatComponent = InStatComponent;
		StatComponent->OnCurrentHPChangeDelegate.AddDynamic(HPBar, &USW_HPBar::OnCurrentHPChange);
		StatComponent->OnMaxHPChangeDelegate.AddDynamic(HPBar, &USW_HPBar::OnMaxHPChange);

		StatComponent->OnCurrentMPChangeDelegate.AddDynamic(MPBar, &USW_MPBar::OnCurrentMPChange);
		StatComponent->OnMaxMPChangeDelegate.AddDynamic(MPBar, &USW_MPBar::OnMaxMPChange);


		UFGameInstance* GameInstance = Cast<UFGameInstance>(GetWorld()->GetGameInstance());
		if (true == ::IsValid(GameInstance)) {
			if (nullptr != GameInstance->GetCharacterStatDataTable() ||
				nullptr != GameInstance->GetCharacterStatDataTableRow(1)) {
				// PlayerController::BeginPlay()가 SStatComponent::BeginPlay()보다 먼저 호출되기 때문에,
				// SStatComponent::BeginPlay()보다 먼저 호출되는 UI에서 초기화한다
				float MaxHP = GameInstance->GetCharacterStatDataTableRow(1)->MaxHP;
				HPBar->SetMaxHP(MaxHP);
				HPBar->InitializeHPBarWidget(StatComponent.Get());

				float NewMaxMP = GameInstance->GetCharacterStatDataTableRow(1)->MaxMP;
				MPBar->SetMaxMP(NewMaxMP);
				MPBar->InitializeMPBarWidget(StatComponent.Get());
			}
		}
	}
}

void UFHUD::BindPlayerState(AFPlayerState* InPlayerState)
{
	if (true == ::IsValid(InPlayerState)) {
		PlayerState = InPlayerState;
		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
	}
}

void UFHUD::BindBuffComponent(UFBuffComponent* InBuffComponent)
{
	if (true == ::IsValid(InBuffComponent)) {
		BuffComponent = InBuffComponent;
	}
}
