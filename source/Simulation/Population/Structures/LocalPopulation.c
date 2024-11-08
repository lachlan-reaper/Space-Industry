#include "LocalPopulation.h"

static int local_population_count = 0;
static LocalPopulation* __static_local_populations = NULL;
void setTransportNodeCountLocalPopulationStatic(const int transport_node_count)
{
	__static_local_populations = (LocalPopulation*) calloc(transport_node_count, sizeof(LocalPopulation));
	local_population_count = transport_node_count;
}

int getLocalPopulationNum(void)
{
	return local_population_count;
}

LocalPopulation* getLocalPopulationByLocation(const TransportNode location)
{
	if (location >= local_population_count) { return NULL; }
	return &__static_local_populations[location];
}

void cleanTransportNodeCountLocalPopulationStatic(void)
{
	free(__static_local_populations);
	local_population_count = 0;
}

static LOCAL_POPULATION_ID_INT id_next = 0;

void assignLocalPopulationValues(const TransportNode location, const uint32_t population_number)
{
	LocalPopulation* population = getLocalPopulationByLocation(location);

	population->population_number = population_number;
	population->employed_number = 0;
	assignFactoryValuesLocalPopulation(&population->population_centre, location);
	population->population_centre.current_employee_num = population->population_centre.max_employee_num;

	population->wealth = 100000;

	population->id = id_next++;
}

// Will not clean or destroy `origin`; also does only a shallow copy
void moveLocalPopulationToStaticArray(LocalPopulation* origin, const TransportNode location)
{
	LocalPopulation* dst = getLocalPopulationByLocation(location);
	cleanLocalPopulation(dst);

	memcpy(dst, origin, sizeof(LocalPopulation));
	dst->population_centre.management = dst;
	reassignOrderOfferingPtrs(&dst->population_centre);
}

void assignLoadIdLocalPopulation(LocalPopulation* obj, const int id)
{
	obj->id = id;
	if (id >= id_next)
	{
		id_next = id + 1;
	}
}

void updateLocalPopulationOfferedPrices(LocalPopulation* population)
{
	for (int i = 0; i < population->population_centre.stockpiles_in_num; i++)
	{
		QUANTITY_INT stockpile_ordered_quantity = 
			population->population_centre.stockpiles_in[i].quantity
			+ *getOrderedInQuantity(
				&population->population_centre, 
				population->population_centre.stockpiles_in[i].product_type
		);

		if (stockpile_ordered_quantity < LP_STOCKPILE_FULL - LP_ORDER_QUANTITY_MIN)
		{		
			if (population->population_centre.orders_in[i].offer_num == 0)
			{
				// Add to market
				if (addBuyOrder(
					getProductMarketAtLocation(population->population_centre.location, population->population_centre.stockpiles_in[i].product_type),
					&population->population_centre.orders_in[i])
				) {
					printf("Failed to add buy order\n");
				}
			}
			population->population_centre.orders_in[i].offer_num = LP_STOCKPILE_FULL - stockpile_ordered_quantity;
		}

		if (population->population_centre.orders_in[i].offer_num > 0)
		{
			ProductMarket* productMarket = getProductMarketAtLocation(population->population_centre.location, population->population_centre.stockpiles_in[i].product_type);

			// TODO TBU
			if (getAvgHistoryWtdAvgArray(&productMarket->buy_hist_array) == 0)
			{
				population->population_centre.orders_in[i].price = (
					LP_DEFAULT_PRICE
					* (sqrt((double)population->population_centre.orders_in[i].offer_num) / LP_DESIRED_BUY_STOCKPILE_ROOT)
				);
			}
			else
			{
				population->population_centre.orders_in[i].price = (
					getAvgHistoryWtdAvgArray(&productMarket->buy_hist_array)
					* (sqrt((double)population->population_centre.orders_in[i].offer_num) / LP_DESIRED_BUY_STOCKPILE_ROOT)
				);
			}

			if (resetBuyOrder(productMarket, &population->population_centre.orders_in[i])) 
			{
				printf("");
				printf("Failed to reset buy order; PM=%x, Order=%x\n", productMarket, &population->population_centre.orders_in[i]);
			}
		}
	}

	for (int i = 0; i < population->population_centre.stockpiles_out_num; i++)
	{
		QUANTITY_INT stockpile_free_quantity = 
			population->population_centre.stockpiles_out[i].quantity
			- *getOrderedOutQuantity(
				&population->population_centre, 
				population->population_centre.stockpiles_out[i].product_type
		);
		if (stockpile_free_quantity > LP_ORDER_QUANTITY_MIN)
		{
			if (population->population_centre.orders_out[i].offer_num == 0)
			{
				// Add to market
				if (addSellOrder(
					getProductMarketAtLocation(population->population_centre.location, population->population_centre.stockpiles_out[i].product_type),
					&population->population_centre.orders_out[i])
				) {
					printf("Failed to add sell order\n");
				}
			}
			population->population_centre.orders_out[i].offer_num = stockpile_free_quantity;
		}

		if (population->population_centre.orders_out[i].offer_num > 0)
		{
			// Re-calc selling price
			ProductMarket* productMarket = getProductMarketAtLocation(population->population_centre.location, population->population_centre.stockpiles_out[i].product_type);
			
			if (getAvgHistoryWtdAvgArray(&productMarket->sell_hist_array) == 0)
			{
				population->population_centre.orders_out[i].price = (
					LP_DEFAULT_PRICE
					* (LP_DESIRED_SELL_STOCKPILE_ROOT / sqrt((double)population->population_centre.orders_out[i].offer_num))
				);
			}
			else
			{
				population->population_centre.orders_out[i].price = (
					getAvgHistoryWtdAvgArray(&productMarket->sell_hist_array)
					* (LP_DESIRED_SELL_STOCKPILE_ROOT / sqrt((double)population->population_centre.orders_out[i].offer_num))
				);
			}

			if (resetSellOrder(productMarket, &population->population_centre.orders_out[i])) 
			{
				printf("Failed to reset sell order\n");
			}
		}
	}
}

void loadLocalPopulationAssignOrders(LocalPopulation* population)
{
	for (int i = 0; i < population->population_centre.stockpiles_in_num; i++)
	{
		QUANTITY_INT stockpile_ordered_quantity = 
			population->population_centre.stockpiles_in[i].quantity
			+ *getOrderedInQuantity(
				&population->population_centre, 
				population->population_centre.stockpiles_in[i].product_type
		);

		if (LP_STOCKPILE_FULL - LP_ORDER_QUANTITY_MIN > stockpile_ordered_quantity)
		{
			// Add to market
			 if (addBuyOrder(
				getProductMarketAtLocation(population->population_centre.location, population->population_centre.stockpiles_in[i].product_type),
				&population->population_centre.orders_in[i])
			) {
				printf("Failed to add buy order\n");
			}
			population->population_centre.orders_in[i].offer_num = CO_STOCKPILE_FULL - stockpile_ordered_quantity;
		}
		else 
		{
			population->population_centre.orders_in[i].offer_num = 0;
		}
	}

	for (int i = 0; i < population->population_centre.stockpiles_out_num; i++)
	{
		QUANTITY_INT stockpile_free_quantity = 
			population->population_centre.stockpiles_out[i].quantity
			- *getOrderedOutQuantity(
				&population->population_centre, 
				population->population_centre.stockpiles_out[i].product_type
		);
		
		if (LP_ORDER_QUANTITY_MIN < stockpile_free_quantity)
		{
			// Add to market
			if (addSellOrder(
				getProductMarketAtLocation(population->population_centre.location, population->population_centre.stockpiles_out[i].product_type),
				&population->population_centre.orders_out[i])
			) {
				printf("Failed to add sell order\n");
			}
			population->population_centre.orders_out[i].offer_num = stockpile_free_quantity;
		}
		else 
		{
			population->population_centre.orders_out[i].offer_num = 0;
		}
	}
}

void insertFundsLocalPopulation(LocalPopulation* population, const int funds)
{
	population->wealth += funds;

	recordInsertFundsFactory(&population->population_centre, funds);
}

void withdrawFundsLocalPopulation(LocalPopulation* population, const int funds)
{
	if (population->wealth < funds) return; // reject payment
	population->wealth -= funds;

	recordWithdrawFundsFactory(&population->population_centre, funds);
}

// TODO TBU
int increaseEmployedLocalPopulation(LocalPopulation* population, const int jobs)
{
	if (population->employed_number + jobs > population->population_number) { return FALSE; }
	population->employed_number += jobs;
	return TRUE;
}

// TODO TBU
int decreaseEmployedLocalPopulation(LocalPopulation* population, const int jobs)
{
	// if (population->employed_number - jobs < 0) { return FALSE; } // SHOULD NEVER HAPPEN
	population->employed_number -= jobs;
	return TRUE;
}

// TODO TBU: CUSTOM MANAGEMENT OF FACTORY
void processTickLocalPopulation(LocalPopulation* population)
{
	population->population_centre.processing_speed = population->population_number / CONSUMPTION_RATE_DIVISOR;
	processTickFactory(&population->population_centre);
	updateLocalPopulationOfferedPrices(population);
}

void cleanLocalPopulation(LocalPopulation* population)
{
	cleanFactory(&population->population_centre);
}
