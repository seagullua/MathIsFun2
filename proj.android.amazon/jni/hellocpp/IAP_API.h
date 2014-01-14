#include <string>
#include "IAP_Event_API.h"

void IAP_storeOpening();
void IAP_storeClosing();
void IAP_initialize(const std::string&);
void IAP_buyMarketItem(const std::string&);
void IAP_restoreTransactions();

