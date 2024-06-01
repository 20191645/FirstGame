// FPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPlayerController.generated.h"

UCLASS()
class FIRSTGAME_API AFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	class UFHUD* GetHUDWidget() const { return HUDWidget; }

	// 메뉴 위젯 토글하기 위해 호출하는 함수
	void ToggleMenu();

	// NPCAmount Getter, Setter 함수
	int32 GetNPCAmount() { return NPCAmount; }
	void SetNPCAmount(int32 NewNPCAmount) { NPCAmount = NewNPCAmount; }

protected:
	virtual void BeginPlay() override;

protected:
	// UserWidget 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFPlayerController")
	TSubclassOf<class UUserWidget> MenuUIClass;
	// UserWidget 인스턴스(개체) <- UserWidget 클래스를 토대로 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFPlayerController")
	TObjectPtr<class UUserWidget> MenuUIInstance;

	// 메뉴 위젯 켜져있는지 확인
	bool bIsMenuOn = false;

private:
	// HUDWidget 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AFPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<class UFHUD> HUDWidgetClass;
	// HUDWidget 인스턴스(개체) <- HUDWidget 클래스를 토대로 생성
	UPROPERTY()
	TObjectPtr<class UFHUD> HUDWidget;

	// NPC 캐릭터 숫자
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AFPlayerController", meta = (AllowPrivateAccess))
	int32 NPCAmount;
};
