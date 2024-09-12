// No Copyright!


#include "UI/Widgets/Cpp_WGT_Master.h"

void UCpp_WGT_Master::SetWidgetController(UObject* Controller) {
	WidgetController = Controller;
	OnWidgetControllerSet();
}