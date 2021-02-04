#pragma once
#include "WindowBase.h"

#include "Widgets/SCompoundWidget.h"

// Forward declaration
class FAlphaViewClient;
class FCameraViewClient;
class FKeyingViewClient;
class FSceneViewClient;
class SPrevisViewport;

class SViewportLayout : public SCompoundWidget
{
public:
    SViewportLayout();
    ~SViewportLayout();

private:
    // Create agument for our custom slate widget (argument are called with SNew(<widgetname>).<argumentname>)
    SLATE_BEGIN_ARGS(SViewportLayout) {}
    SLATE_ARGUMENT(TSharedPtr<class ACameraActor>, mainCamera)
    SLATE_ARGUMENT(TSharedPtr<class UMaterialInstanceDynamic>, keyingMaterialInstance)
    SLATE_ARGUMENT(TSharedPtr<class UMaterialInstanceDynamic>, alphaMaterialInstance)
    SLATE_END_ARGS()

    // Create the content of the custom widget
    void Construct(const FArguments& InArgs);

    TSharedPtr<SPrevisViewport> viewport1;
    TSharedPtr<SPrevisViewport> viewport2;
    TSharedPtr<SPrevisViewport> viewport3;
    TSharedPtr<SPrevisViewport> viewport4;

    TSharedPtr<FAlphaViewClient> alphaViewClient;
    TSharedPtr<FCameraViewClient> cameraViewClient;
    TSharedPtr<FKeyingViewClient> keyingViewClient;
    TSharedPtr<FSceneViewClient> sceneViewClient;
};