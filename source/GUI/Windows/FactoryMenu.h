#ifndef FACTORY_MENU_H
#define FACTORY_MENU_H

#include "..\..\History\HistoryIterator.h"
#include "..\..\Core\Core.h"
#include "..\..\Simulation\Production\Structures\Factory.h"
#include "..\..\Simulation\Production\Enums\ProductionRecipe.h"
#include "..\..\Simulation\Production\Enums\Product.h"
#include "..\..\Simulation\Environment\Enums\TransportNode.h"

#define BUF_SIZE 64

void drawFactoryMenu(AppPlatform* platform, Factory* factory, char* name);

#endif