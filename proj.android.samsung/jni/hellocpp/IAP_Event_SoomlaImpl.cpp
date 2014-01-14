#include "IAP_Event_API.h"
#include "StoreBridge/cocos2dx_EventHandlers.h"

void IAP_Event_billingSupported(bool support)
{
	if(support)
	{
		cocos2dx_EventHandlers::getInstance()->billingSupported();
	}
	else
	{
		cocos2dx_EventHandlers::getInstance()->billingNotSupported();
	}
}
void IAP_Event_marketPurchase(const std::string& item)
{
	std::string s = item;
	cocos2dx_EventHandlers::getInstance()->marketPurchase(s);
}
void IAP_Event_marketPurchaseCancelled(const std::string& item)
{
	std::string s = item;
	cocos2dx_EventHandlers::getInstance()->marketPurchaseCancelled(s);
}
void IAP_Event_restoreTransactions(bool success)
{
	cocos2dx_EventHandlers::getInstance()->restoreTransactions(success);
}