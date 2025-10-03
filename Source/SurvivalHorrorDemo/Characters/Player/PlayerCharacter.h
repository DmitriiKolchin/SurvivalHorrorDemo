// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

USTRUCT()
struct FPlayerCharacterInputs
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> TurnCameraInputAction = nullptr;

};

UCLASS()
class SURVIVALHORRORDEMO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FPlayerCharacterInputs PlayerInputs;

private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void TurnCamera(const FInputActionValue& Value);


};
