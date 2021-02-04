#include "ShotsSystemTab.h"
#include "Widgets/Layout/SScrollBox.h"


#include "PrevisUIEditor/ShotsSystem/ShotsContainer/ShotsContainer.h"

#include "Engine.h"

// todo remove for debug
#include "UnrealEdGlobals.h"
#include "PrevisUIEditor/ShotsSystem/ShotCineCamera/ShotCineCamera.h"

#include "Editor/UnrealEdEngine.h"

#include "Engine/World.h"
#include "PrevisUIEditor/MediaPlane/PlaneKeyer.h"

#define LOCTEXT_NAMESPACE "ShotSystemTab"

SShotSystemTab::SShotSystemTab()
{
	//FWorldDelegates::LevelAddedToWorld.AddStatic(SShotSystemTab::OnChangedLevelHandle); todo
	currentShot = nullptr;
}

void SShotSystemTab::Construct(const FArguments& _inArgs)
{
	tool = _inArgs._Tool;
	currentShot = nullptr;
	detailsViewArgs = FDetailsViewArgs(true, false, true, FDetailsViewArgs::ActorsUseNameArea, false, GUnrealEd);

	InitDetailView();

	
	RefreshContainer();
	ChildSlot
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(0)
			[
				/// === Start Main action
				/// 
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				.Padding(2)
				[
					SNew(SButton)
					.OnClicked(this, &SShotSystemTab::OnAddShot)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Add Shot"))
						.ToolTipText(LOCTEXT("AddShotLabel", "Add shot btn"))
					]
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoWidth()
				.Padding(2)
				[
					SNew(SButton)
					.OnClicked(this, &SShotSystemTab::OnFixActor)
					[
						SNew(STextBlock)
						.Text(FText::FromString("FixShotActor (WIP)"))
						.ToolTipText(LOCTEXT("FixShotLabel", "FixShot btn"))
						.IsEnabled(false)
					]
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				.Padding(2)
				[  
					SNew(SComboBox<TSharedPtr<FName>>)
					.OptionsSource(&shotsNames)
					.OnGenerateWidget(this, &SShotSystemTab::HandleShotsNamesComboBoxGenerateWidget)
					.OnSelectionChanged(this, &SShotSystemTab::HandleShotNameComboBoxSelectionChanged)
					.OnComboBoxOpening(this, &SShotSystemTab::OnComboOpening)
					[
						SNew(STextBlock)
						.Text(this, &SShotSystemTab::HandleShotNameComboBoxContentText)
					]
				]
			]
			
			/// === Start Properties / shot ===
			/// 
			+ SScrollBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(5)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Details :"))
			]
			+ SScrollBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(5)
			[
				detailsView.ToSharedRef()
			]
		];
}

FReply SShotSystemTab::OnAddShot()
{
	RefreshContainer();
	if (!container) return FReply::Handled();
	container->CreateNewShot();
	
	return FReply::Handled();
}

FReply SShotSystemTab::OnFixActor()
{
	RefreshContainer();
	if (!container) return FReply::Handled();

	shotsNames.Reset();
	UpdateShotsNames(container);
	return FReply::Handled();
}

void SShotSystemTab::InitDetailView()
{
	if (!detailsView)
	{
		detailsView = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(detailsViewArgs);
	}

	detailsView->SetEnabled(IsValid(currentShot));

	if (!IsValid(currentShot)) return;
	detailsView->SetObject(currentShot, true);
}

void SShotSystemTab::OnChangedLevelHandle(ULevel*, UWorld*)
{
	// todo : event never not call
}

TArray<TSharedPtr<FName>> SShotSystemTab::GetShotNames() const
{
	if (!container) return TArray<TSharedPtr<FName>>();

	return shotsNames;
}

AShotsContainer* SShotSystemTab::GetContainer() const
{
	AShotsContainer* _shotContainer = nullptr;
	auto _world = GEditor->GetEditorWorldContext().World();

	for (AActor* _actor : _world->GetCurrentLevel()->Actors)
	{
		if (!_actor) continue;

		_shotContainer = Cast<AShotsContainer>(_actor);

		if (_shotContainer != nullptr)
		{
			return _shotContainer;
		}
	}
	return nullptr;
}


void SShotSystemTab::UpdateShotsNames(AShotsContainer* _container)
{
	shotsNames.Empty();
	if (_container) return;

	TArray<AShotCineCamera*> _temp = container->GetShots();
	for (AShotCineCamera* _shot : _temp)
	{
		TSharedPtr<FName> _shotName(new FName(_shot->GetName()));
		shotsNames.Add(_shotName);
	}
}

void SShotSystemTab::RefreshContainer()
{
	auto _container = GetContainer();

	if (!_container)
	{
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		container = GEditor->GetEditorWorldContext().World()->SpawnActor<AShotsContainer>(Location, Rotation, SpawnInfo);
		return;
	}
	container = _container;
	UpdateShotsNames(container);
}


/////////////////////////////////////////  For ComboBoxShots

TSharedRef<SWidget> SShotSystemTab::HandleShotsNamesComboBoxGenerateWidget(TSharedPtr<FName> _stringItem)
{
	FName DefaultName = _stringItem.IsValid() ? *_stringItem : NAME_None;
	return SNew(STextBlock).Text(FText::FromName(DefaultName));
}

void SShotSystemTab::OnComboOpening()
{
	currentShot = nullptr;
	shotsNames.Reset();
	RefreshContainer();
	InitDetailView();

	if(!IsValid(container)) return;
	TArray<AShotCineCamera*> _temp = container->GetShots();
	for (AShotCineCamera* _shot : _temp)
	{
		TSharedPtr<FName> _shotName(new FName(_shot->GetName()));
		shotsNames.Add(_shotName);
	}
	
}

FText SShotSystemTab::HandleShotNameComboBoxContentText() const
{
	AShotsContainer* _container = GetContainer();
	if(!IsValid(_container)) return FText(FText::FromString("Empty"));
	if(_container->GetShots().Num() < 1) return FText(FText::FromString("Empty"));
	if(currentShotName.IsEmpty()) return FText(FText::FromString("Empty"));
	
	return FText::FromString(currentShotName);
}

void SShotSystemTab::HandleShotNameComboBoxSelectionChanged(TSharedPtr<FName> _stringItem, ESelectInfo::Type _selectInfo)
{
	GEditor->SelectNone(false, true); // clear focus
	currentShot = nullptr;
	InitDetailView();
	if (!_stringItem) return;

	currentShotName = _stringItem.Get()->ToString();
	
	// search for select
	TArray<AShotCineCamera*> _temp = container->GetShots();
	for (AShotCineCamera* _shot : _temp)
	{
		if (!_shot) continue;

		if (_shot->GetName() == _stringItem.Get()->ToString())
		{
			currentShot = container->GetShotByName(_shot->GetName());
			GEditor->SelectActor(_shot, true, true);
			InitDetailView();
		}
	}
}

///////////////////////////////////////// End ComboBoxShots

#undef LOCTEXT_NAMESPACE