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

protected:
	virtual void BeginPlay() override;

private:
	// HUDWidget 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AFPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<class UFHUD> HUDWidgetClass;
	// HUDWidget 인스턴스(개체) <- HUDWidget 클래스를 토대로 생성
	UPROPERTY()
	TObjectPtr<class UFHUD> HUDWidget;
};
