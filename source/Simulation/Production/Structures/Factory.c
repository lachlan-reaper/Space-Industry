#include "Factory.h"

static FACTORY_ID_INT id_next = 0;

Factory* newFactory(const ProductionRecipe productionRecipe, const TransportNode location)
{
	Factory* factory = (Factory*) calloc(1, sizeof(Factory));

	assignFactoryValues(factory, productionRecipe, location);

	return factory;
}

void assignFactoryValues(Factory* factory, const ProductionRecipe productionRecipe, const TransportNode location)
{
	loadFactoryConstructor(factory, productionRecipe);

	factory->location = location;
	factory->processing_speed = 1;
	factory->wealth = 1000; // TODO adjust for circumstance

	Stockpile* tmp_arr = getInputs(productionRecipe);
	for (int i = 0; i < factory->stockpiles_in_num; i++) {
		assignStockpileValues(&factory->stockpiles_in[i], tmp_arr[i].product_type, 0);
		assignOrderValues(&factory->orders_in[i], factory, 0, 100); // TODO: DEF. PRICE
		factory->ordered_in[i] = 0;
	}

	tmp_arr = getOutputs(productionRecipe);
	for (int i = 0; i < factory->stockpiles_out_num; i++) {
		assignStockpileValues(&factory->stockpiles_out[i], tmp_arr[i].product_type, 0);
		assignOrderValues(&factory->orders_out[i], factory, 0, 100); // TODO: DEF. PRICE
		factory->ordered_out[i] = 0;
	}

	assignHistoryArrayValues(&factory->profit_history);
	factory->id = id_next++;
}

void loadFactoryConstructor(Factory* factory, const ProductionRecipe productionRecipe)
{
	factory->productionRecipe = productionRecipe;
	
	factory->stockpiles_in_num = getNumOfInputs(productionRecipe);
	factory->stockpiles_out_num = getNumOfOutputs(productionRecipe);

	factory->stockpiles_in = (Stockpile*) realloc(factory->stockpiles_in, factory->stockpiles_in_num * sizeof(Stockpile));
	factory->stockpiles_out = (Stockpile*) realloc(factory->stockpiles_out, factory->stockpiles_out_num * sizeof(Stockpile));

	factory->orders_in = (Order*) realloc(factory->orders_in, factory->stockpiles_in_num * sizeof(Order));
	factory->orders_out = (Order*) realloc(factory->orders_out, factory->stockpiles_out_num * sizeof(Order));

	factory->ordered_in = (QUANTITY_INT*) realloc(factory->ordered_in, factory->stockpiles_in_num * sizeof(QUANTITY_INT));
	factory->ordered_out = (QUANTITY_INT*) realloc(factory->ordered_out, factory->stockpiles_out_num * sizeof(QUANTITY_INT));
}

void assignLoadIdFactory(Factory* obj, const int id)
{
	obj->id = id;
	if (id >= id_next)
	{
		id_next = id + 1;
	}
}

QUANTITY_INT* getOrderedInQuantity(const Factory* factory, const Product product)
{
	for (int i = 0; i < factory->stockpiles_in_num; i++)
	{
		if (factory->stockpiles_in[i].product_type == product)
		{
			return &factory->ordered_in[i];
		}
	}
	return NULL;
}

QUANTITY_INT* getOrderedOutQuantity(const Factory* factory, const Product product)
{
	for (int i = 0; i < factory->stockpiles_out_num; i++)
	{
		if (factory->stockpiles_out[i].product_type == product)
		{
			return &factory->ordered_out[i];
		}
	}
	return NULL;
}

void addOrderedInQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity)
{
	QUANTITY_INT* result = getOrderedInQuantity(factory, product);
	if (result != NULL)
	{
		if (QUANTITY_INT_MAX - quantity >= *result)
		{
			(*result) += quantity;
		}
	}
}

void addOrderedOutQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity)
{
	QUANTITY_INT* result = getOrderedOutQuantity(factory, product);
	if (result != NULL)
	{
		if (QUANTITY_INT_MAX - quantity >= *result)
		{
			(*result) += quantity;
		}
	}
}

void removeOrderedInQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity)
{
	QUANTITY_INT* result = getOrderedInQuantity(factory, product);
	if (result != NULL)
	{
		if (*result >= quantity)
		{
			(*result) -= quantity;
		}
	}
}

void removeOrderedOutQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity)
{
	QUANTITY_INT* result = getOrderedOutQuantity(factory, product);
	if (result != NULL)
	{
		if (*result >= quantity)
		{
			(*result) -= quantity;
		}
	}
}

void insertFundsFactory(Factory* factory, const int funds)
{
	factory->wealth += funds;
	addToHistoryArray(&factory->profit_history, funds);
}

// TODO check funds are avaliable
void withdrawFundsFactory(Factory* factory, const int funds)
{
	if (factory->wealth < funds) return; // reject payment
	factory->wealth -= funds;
	subFromHistoryArray(&factory->profit_history, funds);
}

void processTickFactory(Factory* factory)
{
	tickHistoryArrayIndex(&factory->profit_history);

	// Get the most the factory can make
	uint_fast16_t max_processing_speed = factory->processing_speed;
	for (int i = 0; i < factory->stockpiles_in_num; i++) 
	{
		if (factory->stockpiles_in[i].quantity < max_processing_speed * getCost(factory->productionRecipe, factory->stockpiles_in[i].product_type))
		{
			max_processing_speed = factory->stockpiles_in[i].quantity / getCost(factory->productionRecipe, factory->stockpiles_in[i].product_type);
		}
	}

	// Process the production steps
	for (int i = 0; i < factory->stockpiles_in_num; i++) 
	{
		processTickStockpile(&factory->stockpiles_in[i]);
		removeQuantity(&factory->stockpiles_in[i], max_processing_speed * getCost(factory->productionRecipe, factory->stockpiles_in[i].product_type));
	}

	for (int i = 0; i < factory->stockpiles_out_num; i++) 
	{
		processTickStockpile(&factory->stockpiles_out[i]);
		addQuantity(&factory->stockpiles_out[i], max_processing_speed * getResult(factory->productionRecipe, factory->stockpiles_out[i].product_type));
	}
}

void cleanFactory(Factory* factory) {
	for (int i = 0; i < factory->stockpiles_in_num; i++) {
		cleanStockpile(&(factory->stockpiles_in[i]));
		cleanOrder(&(factory->orders_in[i]));
	}
	free(factory->stockpiles_in);
	free(factory->orders_in);

	for (int i = 0; i < factory->stockpiles_out_num; i++) {
		cleanStockpile(&factory->stockpiles_out[i]);
		cleanOrder(&(factory->orders_out[i]));
	}
	free(factory->stockpiles_out);
	free(factory->orders_out);

	free(factory->ordered_in);
	free(factory->ordered_out);

	cleanHistoryArray(&factory->profit_history);
}
