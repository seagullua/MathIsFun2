#ifndef COLLECTION_H
#define COLLECTION_H

#include "Level.h"
#include "cocos2d.h"
#include <vector>
#include <string>

typedef std::vector<Level*> LevelsArr;
class Collection
{
public:
    ~Collection();

    /**
     * @brief The collection can be in different states
     */
    enum CollectionState
    {
        Locked,
        Unlocked,
        AllCrowns,
        InShop
    };
    enum Difficulty
    {
        Easy=100,
        Medium=200,
        Hard=300,
        UltraHard=400
    };

    typedef unsigned int CollectionID;
    /**
     * @brief returns array of pointer to Levels
     * @return
     */
    const LevelsArr& getLevels() const;

    /**
     * @brief return the number of stamps obtained by user in current collection
     * @return
     */
    const unsigned int stampsObtained() const;
    /**
     * @brief return the max number of stamps which can be obtained by user
     * in current collection
     * @return
     */
    const unsigned int stampsMax() const;

    /**
     * @brief returns the number of crowns obtained in current collection
     * @return
     */
    const unsigned int crownsObtained() const;

    /**
     * @brief returns the number of stamps needed to unlock the level
     * @return
     */
    const unsigned int stampsToUnlock() const;

    const Difficulty getDifficulty() const;
    const CollectionID getCollectionID() const;
    const CollectionState getCollectionState() const;
    const cocos2d::ccColor3B& getColor() const;

    bool isReadyToBeUnlocked() const;
private:
    friend class RW;
#ifdef RW_BUILD_LEVELS
public:
#endif
    Collection();
    Collection(const Collection&);
    Collection& operator=(const Collection&);

    LevelsArr _levels;
    unsigned int _stamps_obtained;
    unsigned int _max_stamps;
    //Deprecated
    unsigned int _stamps_max;
    unsigned int _crowns_obtained;
    unsigned int _stamps_to_unlock;


    Difficulty _collection_difficulty;

    CollectionID _collection_id;
    CollectionState _state;
    cocos2d::ccColor3B _collection_color;
    bool _collection_free;

    void updateStampsAndCrownsCount();
};

#endif // COLLECTION_H
