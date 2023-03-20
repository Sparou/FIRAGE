// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUStopSprintTask.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUStopSprintTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
    USTUStopSprintTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
