#include "PopupWindow.h"

static void appendPopupWindow(PopupWindow* base_window, PopupWindow* new_window);

static void castCoreData(PopupWindow* window, const WindowTypes window_type, void* coreData)
{
	switch (window->window_type)
	{
	case FACTORY_LIST:
	case LOCAL_POPULATION_LIST:
	case LOCATION_GROUP:
	case LOGISTICS_MANAGER_LIST:
	case MAIN_MENU:
		window->coreData.appState = coreData;
		break;
	
	case FACTORY_MENU:
		window->coreData.factory = coreData;
		break;

	case LOCAL_POPULATION_MENU:
		window->coreData.population = coreData;
		break;

	case LOCATION_MENU:
	case PRODUCT_MARKET_LIST:
		window->coreData.location = *((TransportNode*) coreData);
		break;

	case LOGISTICS_CONTRACT_MENU:
		window->coreData.logisticsContract = coreData;
		break;
	
	case LOGISTICS_MANAGER_MENU:
		window->coreData.logisticsManager = coreData;
		break;

	case ORDER_MENU:
		window->coreData.order = coreData;
		break;

	case PRODUCT_MARKET_MENU:
		window->coreData.productMarket = coreData;
		break;

	case STOCKPILE_MENU:
		window->coreData.stockpile = coreData;
		break;

	case VEHICLE_MENU:
		window->coreData.vehicle = coreData;
		break;
	
	default:
		window->coreData.appState = NULL;
		break;
	}
}

void assignPopupWindowValues(PopupWindow* window, const WindowTypes window_type, void* coreData)
{
	static int id = 0;
	window->window_type = window_type;
	snprintf(window->name, BUF_SIZE, "%d", id++);
	castCoreData(window, window_type, coreData);
	window->next = NULL;
	window->prev = NULL;
}

void drawPopupWindow(PopupWindow* window, AppPlatform* platform)
{
	switch (window->window_type)
	{
	case FACTORY_LIST:
		drawFactoryList(platform, window->coreData.appState, window->name);
		break;
	
	case FACTORY_MENU:
		drawFactoryMenu(platform, window->coreData.factory, window->name);
		break;
	
	case LOCAL_POPULATION_LIST:
		drawLocalPopulationList(platform, window->coreData.appState, window->name);
		break;

	case LOCAL_POPULATION_MENU:
		drawLocalPopulationMenu(platform, window->coreData.population, window->name);
		break;

	case LOCATION_GROUP:
		drawLocationGroup(platform, window->coreData.appState, window->name);
		break;

	case LOCATION_MENU:
		drawLocationMenu(platform, window->coreData.location, window->name);
		break;

	case LOGISTICS_CONTRACT_MENU:
		drawLogisticsContractMenu(platform, window->coreData.logisticsContract, window->name);
		break;

	case LOGISTICS_MANAGER_LIST:
		drawLogisticsManagerList(platform, window->coreData.appState, window->name);
		break;

	case LOGISTICS_MANAGER_MENU:
		drawLogisticsManagerMenu(platform, window->coreData.logisticsManager, window->name);
		break;
	
	case MAIN_MENU:
		drawMainMenu(platform, window->coreData.appState, window->name);
		break;

	case ORDER_MENU:
		drawOrderMenu(platform, window->coreData.order, window->name);
		break;
	
	case PRODUCT_MARKET_LIST:
		drawProductMarketList(platform, window->coreData.location, window->name);
		break;

	case PRODUCT_MARKET_MENU:
		drawProductMarketMenu(platform, window->coreData.productMarket, window->name);
		break;

	case SIMULATION_CONTROL_MENU:
		drawSimulationControlMenu(platform, window->name);
		break;

	case STOCKPILE_MENU:
		drawStockpileMenu(platform, window->coreData.stockpile, window->name);
		break;

	case VEHICLE_MENU:
		drawVehicleMenu(platform, window->coreData.vehicle, window->name);
		break;
	
	default:
		break;
	}
}

PopupWindow* addNewPopupWindow(PopupWindow* first_window, const WindowTypes window_type, void* coreData)
{
	PopupWindow* new_window = calloc(1, sizeof(PopupWindow));

	assignPopupWindowValues(new_window, window_type, coreData);
	appendPopupWindow(first_window, new_window);

	return new_window;
}

static void appendPopupWindow(PopupWindow* base_window, PopupWindow* new_window)
{
	while(base_window->next != NULL)
	{
		base_window = base_window->next;
	}
	base_window->next = new_window;
	new_window->prev = base_window;
}

void removePopupWindow(PopupWindow* old_window)
{
	old_window->prev->next = old_window->next;
	if (old_window->next != NULL)
	{
		old_window->next->prev = old_window->prev;
	}
	free(old_window);
}
