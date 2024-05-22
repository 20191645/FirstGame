// FManualLevelUI.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FManualLevelUI.generated.h"

UCLASS()
class FIRSTGAME_API UFManualLevelUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	// 'NextButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnNextButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFManualLevelUI", meta = (AllowPriaveAccess, BindWidget))
	TObjectPtr<class UButton> NextButton;
};
