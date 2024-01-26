#ifndef ORDER_H
#define ORDER_H

typedef struct OrderDef Order;

#include "..\..\Production\Structures\Factory.h"
#include "..\..\Production\Structures\Stockpile.h"

#include <stdint.h>

typedef struct OrderDef {
    Factory* offering_factory;
    QUANTITY_INT offer_num;
    uint_fast16_t price;

    struct OrderDef* prev_order;
    struct OrderDef* left_order;
    struct OrderDef* right_order;
} Order;

Order* newOrder(const Factory* offering_factory, const QUANTITY_INT offer_num, const uint_fast16_t price);
void assignOrderValues(Order* order, const Factory* offering_factory, const QUANTITY_INT offer_num, 
    const uint_fast16_t price, const Order* prev_order, const Order* left_order, const Order* right_order);


// DONT CLEAN CONSTITUENT
void cleanOrder(Order* order);

#endif