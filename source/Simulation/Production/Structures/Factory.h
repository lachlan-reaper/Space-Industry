#ifndef FACTORY_H
#define FACTORY_H

typedef struct Factory Factory;

#define FACTORY_ID_INT uint_least16_t

#include "..\..\..\History\HistoryArrayAvg.h"
#include "..\Enums\ProductionRecipe.h"
#include "..\Structures\Stockpile.h"
#include "..\..\Markets\Structures\Order.h"
#include "..\..\Environment\Enums\TransportNode.h"

struct Factory
{
	ProductionRecipe productionRecipe;
	TransportNode location;

	uint_fast16_t processing_speed; // TODO TBU

	uint_fast16_t employee_wages;
	uint_fast16_t current_employee_num;
	uint_fast16_t max_employee_num;

	uint_fast8_t stockpiles_in_num;
	uint_fast8_t stockpiles_out_num;
	
	Stockpile* stockpiles_in;
	Stockpile* stockpiles_out;

	struct OrderDef* orders_in;
	struct OrderDef* orders_out;

	QUANTITY_INT* ordered_in;
	QUANTITY_INT* ordered_out;

	HistoryArrayAvg profit_history;
	int wealth;

	FACTORY_ID_INT id;
};

#include "..\..\Population\Structures\LocalPopulation.h"

Factory* newFactory(const ProductionRecipe productionRecipe, const TransportNode location);
void assignFactoryValues(Factory* factory, const ProductionRecipe productionRecipe, const TransportNode location);
void loadFactoryConstructor(Factory* factory, const ProductionRecipe productionRecipe);
void assignLoadIdFactory(Factory* obj, const int id);

QUANTITY_INT* getOrderedInQuantity(const Factory* factory, const Product product);
QUANTITY_INT* getOrderedOutQuantity(const Factory* factory, const Product product);

void addOrderedInQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity);
void addOrderedOutQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity);
void removeOrderedInQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity);
void removeOrderedOutQuantity(Factory* factory, const Product product, const QUANTITY_INT quantity);

void addEmployees(Factory* factory, const int employees);
void removeEmployees(Factory* factory, const int employees);

void insertFundsFactory(Factory* factory, const int funds);
void withdrawFundsFactory(Factory* factory, const int funds);

// TODO TBU
void processTickFactory(Factory* factory);

// DO NOT CLEAN ITEM (ONLY CONSTITUENTS)
void cleanFactory(Factory* factory);

#endif