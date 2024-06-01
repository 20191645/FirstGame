// FPlayerController.cpp

#include "Controllers/FPlayerController.h"
#include "UI/FHUD.h"
#include "Components/FStatComponent.h"
#include "Characters/FRPGCharacter.h"
#include "Game/FPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/FBuffComponent.h"

void AFPlayerController::ToggleMenu()
{
	// 메뉴 위젯이 꺼져있을 때 -> 켜기
	if (false == bIsMenuOn) {
		MenuUIInstance->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(MenuUIInstance->GetCachedWidget());
		SetInputMode(Mode);

		// SetPause(true): 게임 일시 정지
		SetPause(true);
		bShowMouseCursor = true;
	}
	// 메뉴 위젯이 켜져있을 때 -> 끄기
	else {
		MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);

		// SetPause(false): 게임 일시 정지 풀기
		SetPause(false);
		bShowMouseCursor = false;
	}

	bIsMenuOn = !bIsMenuOn;
}

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

			AFRPGCharacter* RPGC = GetPawn<AFRPGCharacter>();
			if (true == ::IsValid(RPGC)) {
				UFStatComponent* StatComponent = RPGC->GetStatComponent();
				if (true == ::IsValid(StatComponent)) {
					HUDWidget->BindStatComponent(StatComponent);
				}

				UFBuffComponent* BuffComponent = RPGC->GetBuffComponent();
				if (true == ::IsValid(BuffComponent)) {
					HUDWidget->BindBuffComponent(BuffComponent);
				}
			}
		}
	}

	// MenuUIClass 클래스를 토대로 MenuUIInstance(개체) 생성
	if (true == ::IsValid(MenuUIClass)) {
		MenuUIInstance = CreateWidget<UUserWidget>(this, MenuUIClass);
		if (true == ::IsValid(MenuUIInstance)) {
			// Viewport에 자리 잡는다
			MenuUIInstance->AddToViewport(3);
			MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 'CurrentStage'에 따른 'NPCAmount' 설정
	AFPlayerState* FPlayerState = GetPlayerState<AFPlayerState>();
	if (true == ::IsValid(FPlayerState)) {
		// Stage 2: NPC 5명
		if (FPlayerState->GetCurrentStage() == 2) {
			SetNPCAmount(3);
		}
	}
}
