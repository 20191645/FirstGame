// FPoisonFloor.cpp

#include "WorldStatics/FPoisonFloor.h"
#include "Characters/FRPGCharacter.h"
#include "Components/FBuffComponent.h"

AFPoisonFloor::AFPoisonFloor()
{
 	PrimaryActorTick.bCanEverTick = false;
}

void AFPoisonFloor::CheckOverlap()
{
	// 플레이어 캐릭터 장판 접촉 확인
	AFRPGCharacter* OwningPlayerCharacter =
		Cast<AFRPGCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (false == ::IsValid(OwningPlayerCharacter)) {
		return;
	}

	UFBuffComponent* BuffComponent = OwningPlayerCharacter->GetBuffComponent();
	if (false == ::IsValid(BuffComponent)) {
		return;
	}

	// 플레이어 캐릭터 버프 갱신
	if (true == IsOverlappingActor(OwningPlayerCharacter)) {
		// 이미 다른 버프가 존재 시 해당 버프 지우기
		if (BuffComponent->GetBuffName() != "Poison") {
			BuffComponent->SetBuffName("Poison");
			BuffComponent->SetCurrentStack(0);
		}
		BuffComponent->SetCurrentStack((BuffComponent->GetCurrentStack()) + 1);
	}
}

void AFPoisonFloor::BeginPlay()
{
	Super::BeginPlay();

	// 2초에 한 번씩 디버프 장판 접촉 확인
	FTimerHandle MyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &ThisClass::CheckOverlap, 2.0f, true);
}