// BTService_DetectPlayerCharacter.cpp

#include "AI/BTService_DetectPlayerCharacter.h"
#include "Controllers/FAIController.h"
#include "Characters/FNPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/FCharacter.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter()
{
	// Behavior Tree에서 보이는 노드 이름 설정
	NodeName = TEXT("DetectPlayerCharacter");
	// TickNode() 함수 호출 주기 설정
	Interval = 1.f;
}

void UBTService_DetectPlayerCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AFAIController* AIC = Cast<AFAIController>(OwnerComp.GetAIOwner());
	if (true == ::IsValid(AIC)) {
		AFNPCharacter* NPC = Cast<AFNPCharacter>(AIC->GetPawn());
		if (true == ::IsValid(NPC)) {
			UWorld* World = NPC->GetWorld();
			if (true == ::IsValid(World)) {
				FVector CenterPosition = NPC->GetActorLocation();
				float DetectRadius = 300.f;
				// 하나 이상의 오브젝트를 가져올거기 때문에 TArray 사용 -- {Multi}
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				// 주위 오브젝트들 감지 -- {Overlap}{Multi}{ByChannel}
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel2,	// = Attack Channel
					FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams
				);

				// 오브젝트가 감지된 경우, Player Character 가려내기
				if (true == bResult) {
					for (auto const& OverlapResult : OverlapResults) {
						AFCharacter* PC = Cast<AFCharacter>(OverlapResult.GetActor());
						if (true == ::IsValid(PC)) {
							// IsPlayerController(): 감지된 Character가 플레이어가 조종하고 있는 폰인지 확인
							if (true == PC->GetController()->IsPlayerController()) {
								// Black Board의 'TargetActor' Key 값을 'PlayerCharacter'로 설정
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFAIController::TargetActorKey, PC);

								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
								DrawDebugPoint(World, PC->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
								DrawDebugLine(World, NPC->GetActorLocation(), PC->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);

								return;
							}
							// (캐릭터를 조종하는 컨트롤러 != 플레이어 컨트롤러) -> NonPlayerCharacter
							else {
								// Black Board의 'TargetActor' Key 값을 nullptr로 설정
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFAIController::TargetActorKey, nullptr);

								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
							}
						}
					}
				}
				else {
					// Black Board의 'TargetActor' Key 값을 nullptr로 설정
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFAIController::TargetActorKey, nullptr);
				}

				DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
			}
		}
	}
}
