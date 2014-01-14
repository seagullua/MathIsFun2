#import "StoreAssets.h"
#import "VirtualCategory.h"
#import "VirtualCurrency.h"
#import "VirtualGood.h"
#import "VirtualCurrencyPack.h"
#import "NonConsumableItem.h"
#import "SingleUseVG.h"
#import "PurchaseWithMarket.h"
#import "PurchaseWithVirtualItem.h"






/**
 * This class represents a single game's metadata.
 * Use this protocol to create your assets class that will be transferred to StoreInfo
 * upon initialization.
 */
@implementation StoreAssets



SingleUseVG* HINT_10;
SingleUseVG* HINT_50;
SingleUseVG* HINT_1000;



NonConsumableItem* BUY_ALL;
NonConsumableItem* UNLOCK_ALL;
NonConsumableItem* BUY_1000;
NonConsumableItem* BUY_2000;
NonConsumableItem* BUY_3000;
NonConsumableItem* BUY_4000;


+ (void)initialize{
    

    
    HINT_10 = [[SingleUseVG alloc] initWithName:@"Hints 10" andDescription:@"" andItemId:@"hint_10" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"hint_10" andPrice:0.99]];
    
    HINT_50 = [[SingleUseVG alloc] initWithName:@"Hints 50" andDescription:@"" andItemId:@"hint_50" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"hint_50" andPrice:1.99]];
    
    HINT_1000 = [[SingleUseVG alloc] initWithName:@"Hints 1000" andDescription:@"" andItemId:@"hint_1000" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"hint_1000" andPrice:4.99]];
    
    BUY_ALL = [[NonConsumableItem alloc] initWithName:@"Buy All" andDescription:@"" andItemId:@"buy_all" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"buy_all" andPrice:1.99]];
    
    UNLOCK_ALL = [[NonConsumableItem alloc] initWithName:@"Unlock All" andDescription:@"" andItemId:@"unlock_all" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"unlock_all" andPrice:0.99]];
    
    BUY_1000 = [[NonConsumableItem alloc] initWithName:@"Buy collection" andDescription:@"" andItemId:@"buy_1000" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"buy_1000" andPrice:0.99]];
    BUY_2000 = [[NonConsumableItem alloc] initWithName:@"Buy collection" andDescription:@"" andItemId:@"buy_2000" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"buy_2000" andPrice:0.99]];
    BUY_3000 = [[NonConsumableItem alloc] initWithName:@"Buy collection" andDescription:@"" andItemId:@"buy_3000" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"buy_3000" andPrice:0.99]];
    BUY_4000 = [[NonConsumableItem alloc] initWithName:@"Buy collection" andDescription:@"" andItemId:@"buy_4000" andPurchaseType:[[PurchaseWithMarket alloc] initWithProductId:@"buy_4000" andPrice:0.99]];
    
}

/**
 * A version for your specific game's store assets
 *
 * This value will determine if the saved data in the database will be deleted or not.
 * Bump the version every time you want to delete the old data in the DB.
 * If you don't bump this value, you won't be able to see changes you've made to the objects in this file.
 *
 * NOTE: You can NOT bump this value and just delete the app from your device to see changes. You can't do this after
 * you publish your application on the market.
 *
 * For example: If you previously created a VirtualGood with name "Hat" and you published your application,
 * the name "Hat will be saved in any of your users' databases. If you want to change the name to "Green Hat"
 * than you'll also have to bump the version (from 0 to 1). Now the new "Green Hat" name will replace the old one.
 */
- (int)getVersion {
    return 0;
}

/**
 * A representation of your game's virtual currency.
 */
- (NSArray*)virtualCurrencies{
    return @[];
}

/**
 * An array of all virtual goods served by your store (all kinds in one array). If you have UpgradeVGs, they must appear in the order of levels.
 */
- (NSArray*)virtualGoods{
    return @[HINT_10,HINT_50,HINT_1000];
}

/**
 * An array of all virtual currency packs served by your store.
 */
- (NSArray*)virtualCurrencyPacks{
    return @[];
}

/**
 * An array of all virtual categories served by your store.
 */
- (NSArray*)virtualCategories{
    return @[];
}

/**
 * You can define non consumable items that you'd like to use for your needs.
 * CONSUMABLE items are usually just currency packs.
 * NON-CONSUMABLE items are usually used to let users purchase a "no-ads" token.
 */
- (NSArray*)nonConsumableItems{
    return @[BUY_ALL,UNLOCK_ALL,BUY_1000,BUY_2000,BUY_3000,BUY_4000];
}

@end