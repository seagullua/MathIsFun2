#ifndef PURCHASEWINDOW_H
#define PURCHASEWINDOW_H
#include "cocos2d.h"
#include "Layers/PopUpWindow.h"
#include "Store.h"
#include "Logic/Collection.h"
class PurchaseWindow : public PopUpWindowStyle
{

public:
    PurchaseWindow(Collection* collection)
        : _collection(collection)
    {}
private:
    void onBuyOneClick()
    {
        std::stringstream ss;
        ss << Store::PREFIX_BUY_COLLECTION << _collection->getCollectionID();
        bool res = Store::buyItem(ss.str());
        if(!res)
        {
            cocos2d::CCLog(("Buy collection failed: "+ss.str()).c_str());
        }
    }
    void onBuyAllClick()
    {
        Store::buyItem(Store::ITEM_BUY_ALL);
    }



    void onCreate(cocos2d::CCNode *parent);
    Collection* _collection;
};

#endif // PURCHASEWINDOW_H
