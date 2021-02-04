#include "AlphaViewClient.h"
#include "Engine/Canvas.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialExpressionConstant4Vector.h"

FAlphaViewClient::FAlphaViewClient() :
	FEditorViewportClient(nullptr, nullptr, nullptr),
	renderingCanvas(NewObject<UCanvas>()),
	renderSize(FVector2D(1920, 1080))
{
	// If there is no material, create a default one
	if (!renderingMaterialInstance)
	{
		UMaterial*_renderingMaterial = NewObject<UMaterial>();
		_renderingMaterial->MaterialDomain = EMaterialDomain::MD_UI;

		UMaterialExpressionConstant4Vector* _defaultColorExpression = NewObject<UMaterialExpressionConstant4Vector>(_renderingMaterial);
		_defaultColorExpression->Constant = FLinearColor(0.05, 0.1, 0.2, 1.0);
		_renderingMaterial->Expressions.Add(_defaultColorExpression);
		_renderingMaterial->EmissiveColor.Expression = _defaultColorExpression;

		// Compile the material
		_renderingMaterial->PreEditChange(NULL);
		_renderingMaterial->PostEditChange();

		renderingMaterialInstance = UMaterialInstanceDynamic::Create(_renderingMaterial, NULL);
	}
}

void FAlphaViewClient::Draw(FViewport* _viewport, FCanvas* _canvas)
{
	// Clear entire canvas
	_canvas->Clear(FLinearColor::Blue);

	renderingCanvas->Init(1024, 1024, nullptr, _canvas);
	renderingCanvas->Update();
	renderingCanvas->K2_DrawMaterial(renderingMaterialInstance, FVector2D(0, 0), FVector2D(1024, 1024), FVector2D(0, 0));
}

void FAlphaViewClient::SetMaterial(UMaterialInstanceDynamic* _renderingMaterial)
{
	renderingMaterialInstance = _renderingMaterial;
}