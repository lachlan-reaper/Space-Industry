#include "ProductMarket.h"

static FACTORY_ID_INT id_next = 0;

ProductMarket* newProductMarket(const TransportNode location, const Product product_type)
{
	ProductMarket* productMarket = (ProductMarket*) calloc(1, sizeof(ProductMarket));

	assignProductMarketValues(productMarket, location, product_type);

	return productMarket;
}

void assignProductMarketValues(ProductMarket* productMarket, const TransportNode location, const Product product_type)
{
	productMarket->location = location;
	productMarket->product_type = product_type;

	assignHistoryWtdAvgArrayValues(&productMarket->sell_hist_array);
	assignHistoryWtdAvgArrayValues(&productMarket->buy_hist_array);

	productMarket->sell_order_num = 0;
	productMarket->sell_order_arr_size = 0;
	productMarket->sell_order_arr = NULL;
	productMarket->buy_order_num = 0;
	productMarket->buy_order_arr_size = 0;
	productMarket->buy_order_arr = NULL;

	productMarket->id = id_next++;
}

int addSellOrder(ProductMarket* productMarket, Order* new_order)
{
	while (productMarket->sell_order_num >= productMarket->sell_order_arr_size) // JUST IN CASE
	{
		Order** tmp_arr = realloc(productMarket->sell_order_arr, (productMarket->sell_order_arr_size+1)*2 * sizeof(Order*));
		if (tmp_arr == NULL)
		{
			printf("Error in memory allocation for PM\n");
			return 1;
		}
		productMarket->sell_order_arr = tmp_arr;
		productMarket->sell_order_arr_size = (productMarket->sell_order_arr_size+1)*2;
	}
	productMarket->sell_order_arr[productMarket->sell_order_num++] = new_order;
	return resetSellOrderIndexed(productMarket, productMarket->sell_order_num - 1);
}

Order* addNewSellOrder(ProductMarket* productMarket, const Factory* offering_factory, const QUANTITY_INT offer_num, const ORDER_PRICE_INT price)
{
	Order* order = newOrder(offering_factory, offer_num, price);
	addSellOrder(productMarket, order);
	return order;
}

int addBuyOrder(ProductMarket* productMarket, Order* new_order)
{
	while (productMarket->buy_order_num >= productMarket->buy_order_arr_size) // JUST IN CASE
	{
		Order** tmp_arr = realloc(productMarket->buy_order_arr, (productMarket->buy_order_arr_size+1)*2 * sizeof(Order*));
		if (tmp_arr == NULL)
		{
			printf("Error in memory allocation for PM\n");
			return 1;
		}
		productMarket->buy_order_arr = tmp_arr;
		productMarket->buy_order_arr_size = (productMarket->buy_order_arr_size+1)*2;
	}
	productMarket->buy_order_arr[productMarket->buy_order_num++] = new_order;
	return resetBuyOrderIndexed(productMarket, productMarket->buy_order_num - 1);
}

Order* addNewBuyOrder(ProductMarket* productMarket, const Factory* offering_factory, const QUANTITY_INT offer_num, const ORDER_PRICE_INT price)
{
	Order* order = newOrder(offering_factory, offer_num, price);
	addBuyOrder(productMarket, order);
	return order;
}

static int find_index(const Order* order_arr[], const int length, const Order* order)
{
	for (int i = 0; i < length; i++)
	{
		if (order_arr[i] == order) return i;
	}
	return -1;
}

int removeBuyOrder(ProductMarket* productMarket, Order* old_order)
{
	int index = find_index(productMarket->buy_order_arr, productMarket->buy_order_num, old_order);
	return removeBuyOrderIndexed(productMarket, index);
}

int removeBuyOrderIndexed(ProductMarket* productMarket, int index)
{
	if (index < 0 || productMarket->buy_order_num <= index) return 1;
	productMarket->buy_order_num--;
	if (productMarket->buy_order_num == index)
	{
		return 0;
	}
	swap_orders_arr(
		&productMarket->buy_order_arr[index], 
		&productMarket->buy_order_arr[productMarket->buy_order_num]
	);
	productMarket->buy_order_arr[index]->offer_num = 0;
	return resetBuyOrderIndexed(productMarket, index);
}

int removeSellOrder(ProductMarket* productMarket, Order* old_order)
{
	int index = find_index(productMarket->sell_order_arr, productMarket->sell_order_num, old_order);
	return removeSellOrderIndexed(productMarket, index);
}

int removeSellOrderIndexed(ProductMarket* productMarket, int index)
{
	if (index < 0 || productMarket->sell_order_num <= index) return 1;
	productMarket->sell_order_num--;
	if (productMarket->sell_order_num == index)
	{
		return 0;
	}
	swap_orders_arr(
		&productMarket->sell_order_arr[index], 
		&productMarket->sell_order_arr[productMarket->sell_order_num]
	);
	productMarket->sell_order_arr[index]->offer_num = 0;
	return resetSellOrderIndexed(productMarket, index);
}

QUANTITY_INT match_orders(ProductMarket* selling_market, Order* selling_order, ProductMarket* buying_market, Order* buying_order)
{
	QUANTITY_INT exchanged_num;
	if (selling_order->offer_num > buying_order->offer_num)
	{
		exchanged_num = buying_order->offer_num;
	}
	else
	{
		exchanged_num = selling_order->offer_num;
	}

	selling_order->offer_num -= exchanged_num;
	buying_order->offer_num -= exchanged_num;

	insertFundsFactory(selling_order->offering_factory, exchanged_num * selling_order->price);
	withdrawFundsFactory(buying_order->offering_factory, exchanged_num * buying_order->price);

	addToHistoryWtdAvgArray(&selling_market->sell_hist_array, exchanged_num * selling_order->price, exchanged_num);
	addToHistoryWtdAvgArray(&buying_market->buy_hist_array, exchanged_num * buying_order->price, exchanged_num);

	if (selling_order->offer_num == 0)
	{
		if (removeSellOrder(selling_market, selling_order)) printf("Failed to remove sell order\n");
	}

	if (buying_order->offer_num == 0)
	{
		if (removeBuyOrder(buying_market, buying_order)) printf("Failed to remove buy order\n");
	}

	return exchanged_num;
}

void swap_orders_arr(Order** order_1, Order** order_2)
{
	Order* tmp = *order_1;
	*order_1 = *order_2;
	*order_2 = tmp;
}

int resetBuyOrder(ProductMarket* productMarket, Order* reset_order)
{
	int index = find_index(productMarket->buy_order_arr, productMarket->buy_order_num, reset_order);
	return resetBuyOrderIndexed(productMarket, index);
}

int resetBuyOrderIndexed(ProductMarket* productMarket, int index)
{
	if (index < 0 || productMarket->buy_order_num <= index) return 1;
	while (1)
	{
		if ((index-1)/2 < 0) break; // Reached top
		else if (productMarket->buy_order_arr[(index-1)/2]->price < productMarket->buy_order_arr[index]->price)
		{
			swap_orders_arr(
				&productMarket->buy_order_arr[(index-1)/2],
				&productMarket->buy_order_arr[index]
			);
			index = (index-1)/2;
			continue;
		}
		else break; // Parent is cheaper
	}
	while (1)
	{
		if ((index*2)+1 >= productMarket->buy_order_num) break; // Reached bottom
		else if (productMarket->buy_order_arr[index]->price < productMarket->buy_order_arr[(index*2)+1]->price)
		{
			swap_orders_arr(
				&productMarket->buy_order_arr[index],
				&productMarket->buy_order_arr[(index*2)+1]
			);
			index = (index*2)+1;
			continue;
		}
		else if ((index*2)+2 >= productMarket->buy_order_num) break; // Reached bottom
		else if (productMarket->buy_order_arr[index]->price < productMarket->buy_order_arr[(index*2)+2]->price)
		{
			swap_orders_arr(
				&productMarket->buy_order_arr[index],
				&productMarket->buy_order_arr[(index*2)+2]
			);
			index = (index*2)+2;
			continue;
		}
		else break; // Children are more expensive
	}
	return 0;
}

int resetSellOrder(ProductMarket* productMarket, Order* reset_order)
{
	int index = find_index(productMarket->sell_order_arr, productMarket->sell_order_num, reset_order);
	return resetSellOrderIndexed(productMarket, index);
}

int resetSellOrderIndexed(ProductMarket* productMarket, int index)
{
	if (index < 0 || productMarket->sell_order_num <= index) return 1;
	while (1)
	{
		if ((index-1)/2 < 0) break; // Reached top
		else if (productMarket->sell_order_arr[(index-1)/2]->price > productMarket->sell_order_arr[index]->price)
		{
			swap_orders_arr(
				&productMarket->sell_order_arr[(index-1)/2],
				&productMarket->sell_order_arr[index]
			);
			index = (index-1)/2;
			continue;
		}
		else break; // Parent is cheaper
	}
	while (1)
	{
		if ((index*2)+1 >= productMarket->sell_order_num) break; // Reached bottom
		else if (productMarket->sell_order_arr[index]->price > productMarket->sell_order_arr[(index*2)+1]->price)
		{
			swap_orders_arr(
				&productMarket->sell_order_arr[index],
				&productMarket->sell_order_arr[(index*2)+1]
			);
			index = (index*2)+1;
			continue;
		}
		else if ((index*2)+2 >= productMarket->sell_order_num) break; // Reached bottom
		else if (productMarket->sell_order_arr[index]->price > productMarket->sell_order_arr[(index*2)+2]->price)
		{
			swap_orders_arr(
				&productMarket->sell_order_arr[index],
				&productMarket->sell_order_arr[(index*2)+2]
			);
			index = (index*2)+2;
			continue;
		}
		else break; // Children are more expensive
	}
	return 0;
}

void processTickProductMarket(ProductMarket* productMarket)
{
	tickHistoryWtdAvgArrayIndex(&productMarket->sell_hist_array);
	tickHistoryWtdAvgArrayIndex(&productMarket->buy_hist_array);
}

void cleanProductMarket(ProductMarket* productMarket)
{
	// (don't delete orders thats for the order holder to do)
	cleanHistoryWtdAvgArray(&productMarket->sell_hist_array);
	cleanHistoryWtdAvgArray(&productMarket->buy_hist_array);
}
