#pragma once

#include "Tab.h"

FText UTab::GetText() const { return Text->GetText(); }
void UTab::SetText(FText NewText) { Text->SetText(NewText); }
void UTab::UpdateTab() const { Switcher->SetActiveWidgetIndex(bActive ? 1 : 0); }
