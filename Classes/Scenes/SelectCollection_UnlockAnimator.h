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
        if(_item.getCollection()->isReadyToBeUnlocked())
        {

            AnimatedMenuItem* current_image = _item.getItem();

            CCScaleTo* logo_scale = CCScaleTo::create(0.5, 1.25f);
            CCFadeTo* logo_fade = CCFadeTo::create(0.25f, 0);
            CCDelayTime* start_delay = CCDelayTime::create(1.5f);
            CCDelayTime* delay = CCDelayTime::create(1.75f);
            CCCallFunc* helper = CCCallFunc::create(
                        this,
                        callfunc_selector(SelectCollection::CollectionUnlockAnimator::unlockHelper));
            CCCallFunc* cleanup = CCCallFunc::create(
                        this,
                        callfunc_selector(SelectCollection::CollectionUnlockAnimator::cleanup));
            current_image->runAction(CCSequence::create(start_delay, logo_scale, NULL));
            current_image->runAction(CCSequence::create(
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


        AnimatedMenuItem* item = _parent->createCollectionItem(_item.getCollection(), _parent->_papers_spl);
        _parent->_tiles[item] = SelectCollection::CollectionTile(item, _item.getCollection());
        item->setPosition(current_image->getPosition());
        //item->setBaseScale(0.7f);
        current_image->getParent()->addChild(item, -5);


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
    AnimatedMenuItem* _to_clean_up;
};

#endif // SELECTCOLLECTIO_UNLOCKANIMATOR_H
