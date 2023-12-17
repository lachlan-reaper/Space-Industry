#ifndef PRODUCTION_RECIPE_H
#define PRODUCTION_RECIPE_H

#include "Stockpile.h"

#include <stdint.h>
#include <stdlib.h>

typedef enum ProductionRecipe 
{
    Refinement_Iron,
	Refinement_Copper,
	Refinement_Carbon,
	Refinement_Silicon,
	Crush_Rock,
	Fractional_Distillation,
	Refinement_Aviation_Fuel,
	Plastic_Production,
	Asphalt_Production,
	Steel_Production,
	Copper_Wire_Production,
	Electrolysis,
	Carbonic_Acid_Production,
	Combustion_Hydrogen,
	Combustion_Coal,
	Combustion_Carbon,
	Combustion_Sulfur,
	Combustion_Crude_Oil,
	Combustion_Petroleum_Gas,
	Combustion_Gasoline,
	Combustion_Kerosene,
	Combustion_Aviation_Fuel,
	Combustion_Diesel
} ProductionRecipe;

char* getName(const ProductionRecipe productionRecipe);
int_fast16_t getEnergy(const ProductionRecipe productionRecipe);
uint_fast8_t getNumOfInputs(const ProductionRecipe productionRecipe);
Stockpile* getInputs(const ProductionRecipe productionRecipe);
uint_fast8_t getNumOfOutputs(const ProductionRecipe productionRecipe);
Stockpile* getOutputs(const ProductionRecipe productionRecipe);
QUANTITY_INT getCost(const ProductionRecipe productionRecipe, const Product product);
QUANTITY_INT getResult(const ProductionRecipe productionRecipe, const Product product);


#endif