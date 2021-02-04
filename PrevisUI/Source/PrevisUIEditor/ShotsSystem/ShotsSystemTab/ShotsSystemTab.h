#pragma once

#include "PrevisUIEditor/ShotsSystem/ShotsContainer/ShotsContainer.h"
#include "Templates/SharedPointer.h"
#include "Widgets/Colors/SColorBlock.h"
class ACineCameraActor;
class IPropertyChangeListener;
class UShotObject;

class SShotSystemTab : public SCompoundWidget, FEditorUndoClient, FNotifyHook
{
private:
	SLATE_BEGIN_ARGS(SShotSystemTab) {}
	SLATE_ARGUMENT(TWeakPtr<class TabTool>, Tool)
	SLATE_ARGUMENT(TArray<UShotObject*>, Shots)
	SLATE_ARGUMENT(AShotsContainer*, Container)
	SLATE_ARGUMENT(TArray<FString>, ShotsNames)
	SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, ShotNameHandle)
	SLATE_END_ARGS()
	
	TWeakPtr<TabTool> tool;
	
	UPROPERTY()
	AShotsContainer* container;
	
	UPROPERTY()
	AShotCineCamera* currentShot;

private:
	AShotsContainer* GetContainer() const;
	void UpdateShotsNames(AShotsContainer* _container);
	void RefreshContainer();

	
private:
	//////// For UI ////////

	///
	/// Property  ///
	/// 
	UPROPERTY()
	TArray<TSharedPtr<FName>> shotsNames;
	
	UPROPERTY()
	FString currentShotName;

	
	TSharedPtr<IDetailsView> detailsView;
	FDetailsViewArgs detailsViewArgs;
	///
	/// Methods ///
	/// 
	TSharedRef<SWidget> HandleShotsNamesComboBoxGenerateWidget(TSharedPtr<FName> StringItem);
	void OnComboOpening();
	FText HandleShotNameComboBoxContentText() const;
	void HandleShotNameComboBoxSelectionChanged(TSharedPtr<FName> _stringItem, ESelectInfo::Type _selectInfo);
	

	///
	/// Test ///
	/// 
	static void OnChangedLevelHandle(ULevel*, UWorld*); // todo

public:
	SShotSystemTab();
	void Construct(const FArguments& _inArgs);

public:
	/// === Getters/Setters
	///
	TArray<TSharedPtr<FName>> GetShotNames() const;


private:
	/// === Callback UI ===
	///
	/// btn
	FReply OnAddShot();
	FReply OnFixActor();
	void InitDetailView();
};