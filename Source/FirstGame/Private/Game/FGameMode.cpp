// FGameMode.cpp

#include "Game/FGameMode.h"
#include "Controllers/FPlayerController.h"
#include "Game/FPlayerState.h"
#include "Characters/FRPGCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/FStatComponent.h"

AFGameMode::AFGameMode()
{
	// 플레이어 컨트롤러 설정 -- Class 정보 초기화
	// Object 생성하지 않고 Class 정보로 지정하는 이유:
	// 멀티플레이 고려 - Class 정보로 플레이어 입장마다 Instance를 생성함
	PlayerControllerClass = AFPlayerController::StaticClass();
}

void AFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// 로그인한 플레이어의 PlayerState 초기화
	AFPlayerState* PlayerState = NewPlayer->GetPlayerState<AFPlayerState>();
	if (true == ::IsValid(PlayerState)) {
		// PlayerState의 델리게이트에 CurrentStageChanged() 멤버 함수 바인드
		PlayerState->OnCurrentStageChangedDelegate.AddDynamic(this, &ThisClass::CurrentStageChanged);
		PlayerState->InitPlayerState();
	}
}

void AFGameMode::CurrentStageChanged(int32 NewCurrentStage)
{
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (true == ::IsValid(PC))
	{
		// PlayerStart Tag(==CurrentStage)로 찾기
		FString SpotName = FString::FromInt(NewCurrentStage);
		AActor* StartSpot = FindPlayerStart(PC, SpotName);
		// 플레이어 캐릭터 위치 이동
		AFRPGCharacter* RPGC = Cast<AFRPGCharacter>(PC->GetCharacter());
		RPGC->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
		// 플레이어 캐릭터 스폰 위치 변경
		RestartPlayerAtPlayerStart(PC, StartSpot);
	}
}