#include "ViewportLayout.h"
#include "PrevisViewport.h"
#include "Clients/AlphaViewClient.h"
#include "Clients/CameraViewClient.h"
#include "Clients/KeyingViewClient.h"
#include "Clients/SceneViewClient.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

SViewportLayout::SViewportLayout() :
    viewport1(SNew(SPrevisViewport)),
    viewport2(SNew(SPrevisViewport)),
    viewport3(SNew(SPrevisViewport)),
    viewport4(SNew(SPrevisViewport)),
    alphaViewClient(MakeShareable(new FAlphaViewClient)),
    cameraViewClient(MakeShareable(new FCameraViewClient)),
    keyingViewClient(MakeShareable(new FKeyingViewClient)),
    sceneViewClient(MakeShareable(new FSceneViewClient)) { }

SViewportLayout::~SViewportLayout() {}

void SViewportLayout::Construct(const FArguments& InArgs)
{
    if (InArgs._mainCamera.IsValid())
    {
        cameraViewClient->SetCamera(InArgs._mainCamera.Get());
    }

    if (InArgs._keyingMaterialInstance.IsValid())
    {
        alphaViewClient->SetMaterial(InArgs._keyingMaterialInstance.Get());
    }

    if (InArgs._alphaMaterialInstance.IsValid())
    {
        keyingViewClient->SetMaterial(InArgs._alphaMaterialInstance.Get());
    }

    // The actual content of the widget
    ChildSlot
    [
        SNew(SScrollBox)
        + SScrollBox::Slot()
        .VAlign(VAlign_Top)
        .Padding(2)
        [
            SNew(SBorder)
            .BorderBackgroundColor(FColor(192, 192, 192, 255))
            .Padding(15.0f)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("This is a tab example.")))
            ]
        ]
        + SScrollBox::Slot()
        .HAlign(HAlign_Left)
        .Padding(2)
        [

            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .VAlign(VAlign_Top)
            .Padding(2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .HAlign(HAlign_Left)
                .Padding(2)
                [
                    SNew(SPrevisViewport)
                    .client(cameraViewClient)
                ]
                + SHorizontalBox::Slot()
                .HAlign(HAlign_Right)
                .Padding(2)
                [
                    SNew(SPrevisViewport)
                    .client(sceneViewClient)
                ]
            ]
            + SVerticalBox::Slot()
            .VAlign(VAlign_Top)
            .Padding(2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .HAlign(HAlign_Right)
                .Padding(2)
                [
                    SNew(SPrevisViewport)
                    .client(keyingViewClient)
                ]
                + SHorizontalBox::Slot()
                .HAlign(HAlign_Right)
                .Padding(2)
                [
                    SNew(SPrevisViewport)
                    .client(alphaViewClient)
                ]
            ]
        ]
    ];
}