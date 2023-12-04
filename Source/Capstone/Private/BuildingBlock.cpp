// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "BuildingBlock.h"

//void ABuildingBlock::HighlightBlock(bool IsInPath)
//{
//    TArray<UStaticMeshComponent*> Components;
//    GetComponents<UStaticMeshComponent>(Components);
//    for (int32 i = 0; i < Components.Num(); i++)
//    {
//        UStaticMeshComponent* StaticMeshComponent = Components[i];
//        //UMaterialInterface* StaticMeshMaterial = StaticMeshComponent->GetStaticMesh()->GetMaterial(0);
//        // 
//        UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(MaterialRef, this);
//        //StaticMeshComponent->GetStaticMesh()->SetMaterial
//        StaticMeshComponent->GetStaticMesh()->SetMaterial(0, DynMaterial);
//        //set paramater with Set***ParamaterValue
//        DynMaterial->SetScalarParameterValue("Opacity", IsInPath ? 1.0 : 0.0);
//    }
//}
