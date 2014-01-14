#include <string>

void IAP_Event_billingSupported(bool support);
void IAP_Event_marketPurchase(const std::string& item);
void IAP_Event_marketPurchaseCancelled(const std::string& item);
void IAP_Event_restoreTransactions(bool success);