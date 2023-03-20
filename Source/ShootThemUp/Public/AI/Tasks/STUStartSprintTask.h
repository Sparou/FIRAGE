// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUStartSprintTask.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUStartSprintTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
    USTUStartSprintTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
