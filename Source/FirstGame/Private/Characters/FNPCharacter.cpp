// FNPCharacter.cpp

#include "Characters/FNPCharacter.h"
#include "Controllers/FAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AFNPCharacter::AFNPCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // 레벨에 새롭게 배치되거나 생성되면 FAIController 빙의가 자동으로 이뤄진다
    AIControllerClass = AFAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AFNPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// IsPlayerControlled(): 플레이어가 컨트롤하고 있는 폰인지 확인
	if (false == IsPlayerControlled()) {
		// bUseControllerRotation***: 컨트롤러 움직임에 따라 캐릭터가 회전하지 않는다 [false]
		bUseControllerRotationYaw = false;

		// 회전 속도를 지정하여 이동 방향으로 캐릭터가 부드럽게 회전하는 기능 추가
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		// NPC 이동 속도 너프
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}
