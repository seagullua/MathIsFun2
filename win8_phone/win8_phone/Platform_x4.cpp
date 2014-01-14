#include "StoreBridge/cocos2dx_EventHandlers.h"
#include "StoreBridge/cocos2dx_StoreController.h"
#include "AppDelegate.h"
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"

void cocos2dx_StoreController::initialize(string)
{}
void cocos2dx_StoreController::restoreTransactions()
{
#ifndef JUNIOR
    unsigned int i = rand() % 2;
    bool res = i == 1;
    AppDelegate::getPurchaseHandler()->restoreTransactions(res);
#endif
}

void cocos2dx_StoreController::buyMarketItem(string item)
{
#ifndef JUNIOR
    AppDelegate::getPurchaseHandler()->marketPurchase(item);
#endif
}
void cocos2dx_StoreController::storeOpening()
{}
void cocos2dx_StoreController::storeClosing()
{}
void Browser::openURL(const URLId id)
{
    cocos2d::CCLog("Open URL: %d", id);
}

void Statistics::statisticEvent(const StatisticsEvent id)
{
    cocos2d::CCLog("Event %d", id);
}

void Statistics::detailStatisticEvent(const StatisticsEvent id, const unsigned int level_id, const unsigned int collection_id, const unsigned int stamps_num)
{
    cocos2d::CCLog("Event %d, Level %d, Collection %d, Stamps %d", id, level_id+1, collection_id, stamps_num);
}

Language::Languages Language::getDeviceLanguage()
{
	
	//Windows::Foundation::Collections::IVectorView<Platform::String^>^ a = Windows::System::UserProfile::GlobalizationPreferences::Languages;
	
	Language::Languages res = Language::English;

	/*for(unsigned i = 0; i<a->Size; ++i)
	{
		Platform::String^ b = a->GetAt(i);
		const wchar_t* s = b->Data();
		if(s[0] == 'u' && s[1] == 'k')
			res = Language::Russian;
		if(s[0] == 'r' && s[1] == 'u')
			res = Language::Russian;

	}*/
	
	return res;
}

void Notification::notify(const Type t)
{
    cocos2d::CCLog("Notification ID %d", t);

}
