// SW_MPBar.h

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_Bar.h"
#include "SW_MPBar.generated.h"

UCLASS()
class FIRSTGAME_API USW_MPBar : public USW_Bar
{
	GENERATED_BODY()

public:
	void SetMaxMP(float InMaxMP);

	void InitializeMPBarWidget(class UFStatComponent* NewStatComponent);

	UFUNCTION()
	void OnMaxMPChange(float InOldMaxMP, float InNewMaxMP);
	UFUNCTION()
	void OnCurrentMPChange(float InOldMP, float InNewMP);

protected:
	virtual void NativeConstruct() override;
};
