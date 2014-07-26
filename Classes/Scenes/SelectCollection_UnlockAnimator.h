#ifndef SELECTCOLLECTIO_UNLOCKANIMATOR_H
#define SELECTCOLLECTIO_UNLOCKANIMATOR_H
#include "SelectCollection.h"
class SelectCollection::CollectionUnlockAnimator :  public CCObject
{
public:
    CollectionUnlockAnimator(const SelectCollection::CollectionTile& t,
                             SelectCollection* parent)
        :_item(t), _parent(parent), _to_clean_up(0)
    {
        this->retain();
        _parent->retain();
    }

    void unlockItem()
    {
        using namespace cocos2d;
        if(_item.getCollection()->isReadyToBeUnlocked())
        {

            ADMenuItem* current_image = _item.getItem();

            cocos2d::CCScaleTo* logo_scale = cocos2d::CCScaleTo::create(0.5, 1.25f);
            cocos2d::CCFadeTo* logo_fade = cocos2d::CCFadeTo::create(0.25f, 0);
            cocos2d::CCDelayTime* start_delay = cocos2d::CCDelayTime::create(1.5f);
            cocos2d::CCDelayTime* delay = cocos2d::CCDelayTime::create(1.75f);
            cocos2d::CCCallFunc* helper = cocos2d::CCCallFunc::create(
                        this,
                        callfunc_selector(SelectCollection::CollectionUnlockAnimator::unlockHelper));
            cocos2d::CCCallFunc* cleanup = cocos2d::CCCallFunc::create(
                        this,
                        callfunc_selector(SelectCollection::CollectionUnlockAnimator::cleanup));
            current_image->runAction(cocos2d::CCSequence::create(start_delay, logo_scale, NULL));
            current_image->runAction(cocos2d::CCSequence::create(
                                         delay, helper, logo_fade, cleanup, NULL));

        }
        else
        {
            _parent->release();
            this->release();
        }
    }

private:
    void unlockHelper()
    {
        CCNode* current_image = _item.getItem();


        ADMenuItem* item = _parent->createCollectionItem(_item.getCollection(), _parent->_papers_spl);
        SelectCollection::CollectionTile tile(item, _item.getCollection());
        _parent->_tiles[item] = tile;
        item->setPosition(current_image->getPosition());
        //item->setBaseScale(0.7f);
        current_image->getParent()->addChild(item, -5);

        SelectCollection* parent = _parent;
        item->setClickAction([tile, parent](){
                    parent->onCollectionSelect(tile);
                });


        _to_clean_up =  _item.getItem();
    }
    void cleanup()
    {
        if(_to_clean_up)
        {
            TileMap::iterator it= _parent->_tiles.find(_to_clean_up);
            if(it != _parent->_tiles.end())
                _parent->_tiles.erase(it);

            _to_clean_up->removeFromParent();
        }
        this->release();
        _parent->release();
    }
    SelectCollection::CollectionTile _item;
    SelectCollection* _parent;
    ADMenuItem* _to_clean_up;
};

#endif // SELECTCOLLECTIO_UNLOCKANIMATOR_H
