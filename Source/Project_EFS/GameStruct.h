#pragma once

#include "CoreMinimal.h"
#include "GameStruct.generated.h"

USTRUCT()
struct FItemStruct
{
    GENERATED_USTRUCT_BODY()
        UPROPERTY()
        uint32 ItemID;

        TTuple<int, int> itemSize;
};