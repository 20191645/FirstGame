// FPlayerController.cpp

#include "Controllers/FPlayerController.h"
#include "UI/FHUD.h"
#include "Components/FStatComponent.h"
#include "Characters/FRPGCharacter.h"
#include "Game/FPlayerState.h"

void AFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// SetInputMode(): 플레이 버튼 누르면 바로 포커싱 되게끔 함
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if (true == ::IsValid(HUDWidgetClass)) {
		HUDWidget = CreateWidget<UFHUD>(this, HUDWidgetClass);
		if (true == ::IsValid(HUDWidget)) {
			HUDWidget->AddToViewport();

			AFPlayerState* FPlayerState = GetPlayerState<AFPlayerState>();
			if (true == ::IsValid(FPlayerState)) {
				HUDWidget->BindPlayerState(FPlayerState);
			}

			AFCharacter* PC = GetPawn<AFCharacter>();
			if (true == ::IsValid(PC)) {
				UFStatComponent* StatComponent = PC->GetStatComponent();
				if (true == ::IsValid(StatComponent)) {
					HUDWidget->BindStatComponent(StatComponent);
				}
			}
		}
	}
}
