#ifndef SAVE_FILE_STANDARDS_H
#define SAVE_FILE_STANDARDS_H

#define SAVE_FILE_SEP '='
#define SAVE_FILE_PTR_PREFIX '>'
#define SAVE_FILE_ID_SEP '_'
#define SAVE_FILE_ATTR_ID '\t'
#define SAVE_FILE_NEXT_ATTR_SEP '\n'

#define SAVE_FILE_APP_STATE_ID "AppState"
#define SAVE_FILE_FACTORY_MANAGER_ID "FactoryManager"
#define SAVE_FILE_FACTORY_ID "Factory"
#define SAVE_FILE_LOGISTICS_CONTRACT_ID "LogisticsContract"
#define SAVE_FILE_LOGISTICS_MANAGER_ID "LogisticsManager"
#define SAVE_FILE_ORDER_ID "Order"
#define SAVE_FILE_STOCKPILE_ID "Stockpile"
#define SAVE_FILE_VEHICLE_ID "Vehicle"

#define SAVE_FILE_AS_LOG_MAN_NUM "logistics_managers_num"
#define SAVE_FILE_AS_LOG_MAN_ID "logistics_managers"
#define SAVE_FILE_AS_FAC_MAN_NUM "factory_managers_num"
#define SAVE_FILE_AS_FAC_MAN_ID "factory_managers"

#define SAVE_FILE_FM_CON_FAC_ID "controlled_factory"

#define SAVE_FILE_FAC_PRO_REC_ID "productionRecipe"
#define SAVE_FILE_FAC_STO_IN_ID "stockpiles_in"
#define SAVE_FILE_FAC_STO_OUT_ID "stockpiles_out"
#define SAVE_FILE_FAC_ORD_IN_ID "orders_in"
#define SAVE_FILE_FAC_ORD_OUT_ID "orders_out"
#define SAVE_FILE_FAC_ORD_NUM_IN_ID "ordered_in"
#define SAVE_FILE_FAC_ORD_NUM_OUT_ID "ordered_out"

#define SAVE_FILE_FAC_LOC_ID "location"
#define SAVE_FILE_FAC_PRO_SPE_ID "processing_speed"

#define SAVE_FILE_LOG_MAN_VEH_NUM "vehicles_num"
#define SAVE_FILE_LOG_MAN_VEH_ID "vehicles"
#define SAVE_FILE_LOG_MAN_CON_NUM "contracts_num"
#define SAVE_FILE_LOG_MAN_CON_ID "contracts"

#define SAVE_FILE_LOG_CON_VEH_ID "assigned_vehicle"
#define SAVE_FILE_LOG_CON_SEL_FAC_ID "selling_factory"
#define SAVE_FILE_LOG_CON_BUY_FAC_ID "buying_factory"
#define SAVE_FILE_LOG_CON_CUR_PHA_ID "current_phase"
#define SAVE_FILE_LOG_CON_PRO_ID "product"
#define SAVE_FILE_LOG_CON_QUA_ID "quantity"

#define SAVE_FILE_ORD_OFF_FAC_ID "offering_factory"
#define SAVE_FILE_ORD_OFF_NUM_ID "offer_num"
#define SAVE_FILE_ORD_PRI_ID "price"

#define SAVE_FILE_STO_PRO_ID "product_type"
#define SAVE_FILE_STO_QUA_ID "quantity"

#define SAVE_FILE_VEH_STO_ID "stockpile"

#define SAVE_FILE_VEH_CUR_LOC_ID "current_location"
#define SAVE_FILE_VEH_END_LOC_ID "end_location"
#define SAVE_FILE_VEH_DIS_TRA_ID "distance_travelled"
#define SAVE_FILE_VEH_END_FAC_ID "end_factory"
#define SAVE_FILE_VEH_MAX_CAP_ID "max_capacity"

#define BUF_SIZE 128

enum AttributeTypes {
	APP_STATE_SAVE,
	FACTORY_MANAGER_SAVE,
	FACTORY_SAVE,
	LOGISTICS_CONTRACT_SAVE,
	LOGISTICS_MANAGER_SAVE,
	ORDER_SAVE,
	STOCKPILE_SAVE,
	VEHICLE_SAVE
};

#endif