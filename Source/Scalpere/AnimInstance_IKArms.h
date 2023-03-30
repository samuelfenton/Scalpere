// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MotionControllerComponent.h"

#include "AnimInstance_IKArms.generated.h"

UCLASS()
class SCALPERE_API UAnimInstance_IKArms : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	UAnimInstance_IKArms();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Assigned Controllers")
	UMotionControllerComponent* m_rightController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Assigned Controllers")
	UMotionControllerComponent* m_leftController;
};
