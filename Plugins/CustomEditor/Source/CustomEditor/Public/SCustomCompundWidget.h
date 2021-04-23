// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Reply.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Implementation of main plugin editor Slate widget
 */
class SCustomCompundWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SCustomCompundWidget)
	{
	}

	SLATE_END_ARGS()

	/** Destructor */
	virtual ~SCustomCompundWidget();

	/** Widget constructor */
	void Construct( const FArguments& Args );
};

