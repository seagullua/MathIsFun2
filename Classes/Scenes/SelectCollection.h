#ifndef SELECTCOLLECTION_H
#define SELECTCOLLECTION_H
#include "cocos2d-A.h"
#include "Logic/Collection.h"
#include <vector>
#include "Layers/PopUpWindow.h"
#include <ADLib/Device/ADAds.h>
#include "SceneStyle.h"
#include <ADLib/Rendering/ADScrollView.h>

class SelectCollection : public SceneStyle
{
public:
    enum Mode
    {
        Collections,
        Shop
    };

    SelectCollection(const Mode mode = Collections);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(const Mode mode = Collections);
    static SelectCollection* create(const Mode mode = Collections);


    void onCollectionSelect(CCObject* pSender);
    static void purchaseReload();
    static void purchaseOpenCollections();
private:
    static SelectCollection* _last_scene_ptr;

    class CollectionTile
    {
    public:

        CollectionTile(ADMenuItem* item=0,
                       Collection* collection = 0,
                       bool is_shop_tile=false)
            : _item(item), _collection(collection), _is_shop_tile(is_shop_tile)
        {}

        ADMenuItem* getItem() const
        {
            return _item;
        }
        Collection* getCollection() const
        {
            return _collection;
        }
        bool isShopTile() const
        {
            return _is_shop_tile;
        }
    private:
        ADMenuItem* _item;
        Collection* _collection;
        bool _is_shop_tile;
    };
    class CollectionUnlockAnimator;

    void onBackClick();

    void doGoBack();
    void doOpenCollection();
    void doOpenCollections();
    void doOpenShop();
    void doReload();

    void hideEverything(cocos2d::CCCallFunc *callback);
    void buildCollectionTiles();
    void newScrolling(MenuSpriteBatch* menu, float width);

    ADMenuItem* createCollectionItem(Collection* collection,
                                           const SpritesLoader& spl);
    ADMenuItem* createCollectionItem(const std::string& image,
                                           const std::string& label,
                                           const std::string& stamps_label,
                                           const cocos2d::ccColor3B& color,
                                           const SpritesLoader& spl,
                                           const std::string& stamp_file_name,
                                           bool show_crown,
                                           bool show_new,
                                           bool shop_mode);


    cocos2d::CCSprite* _title_select_collection;
    typedef std::map<ADMenuItem*, CollectionTile> TileMap;
    TileMap _tiles;
    MenuSpriteBatch* _collections_menu;
    SpritesLoader _col_spl;
    Collection* _last_selected_collection;
    SpritesLoader _papers_spl;

    static cocos2d::CCPoint _last_scroll_view_offset;
    ADScrollView* _collections_scroll_view;
    Mode _mode;

    class UnlockWindow;
    class PurchaseWindow;

    ADAds::Banner* _bottom_banner;
public:
    CREATE_FUNC(SelectCollection)
};

#endif // SELECTCOLLECTION_H
