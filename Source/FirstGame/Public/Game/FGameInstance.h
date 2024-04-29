// FGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "FGameInstance.generated.h"

// .csv 파일을 읽어오기 위해 FTableRowBase 구조체 이용
// -- 'DT_StatTable.csv' 파일의 각 열 이름(MaxHP)과 동일한 구조체
USTRUCT(BlueprintType)
struct FFStatTableRow :public FTableRowBase {
	GENERATED_BODY()

public:
	FFStatTableRow() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMP;
};

UCLASS()
class FIRSTGAME_API UFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 게임이 시작되면 자동으로 실행
	virtual void Init() override;
	// 게임이 끝나면 자동으로 실행
	virtual void Shutdown() override;

	// DataTable 반환
	const UDataTable* GetCharacterStatDataTable() { return CharacterStatDataTable; }
	// DataTable의 Row 번호를 입력으로 받아서 Row 내용 반환
	FFStatTableRow* GetCharacterStatDataTableRow(int32 InStage);

private:
	// DataTable('DT_Character')의 데이터를 가져올 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UFGameInstance", meta = (AllowPrivateAccess))
	class UDataTable* CharacterStatDataTable;
};