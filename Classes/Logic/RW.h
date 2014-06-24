#ifndef RW_H
#define RW_H
#include "Collection.h"
#include "Level.h"
#include <map>
typedef std::map<Collection::CollectionID, Collection*> CollectionsArr;
class Loading;
class RW
{
public:
    static const CollectionsArr& getCollections();
    static unsigned int allStampsObtained();
    //static unsigned int allStampsMax();
    static unsigned int allCrownsObtained();
    /**
     * @brief Returns 0 if the given Level is the last in collection
     * @param l
     * @return
     */
    static Level* getNextLevel(Level* l);
    static Level* skipLevel(Level* l);
    //Returns true if this is new solution and false otherwise
    static bool registerSolution(Level* l, const Solution&);
    static bool unlockCollection(Collection* col);
    static void deletePersistentInfo();
    static bool isExpertMode();
    //static bool hasTranslationPollAlreadyShown();
    static bool isAdsDisabled();
    //static void pollIsShown();
    static void setExpertMode(bool expert);
    static void flushSettings();

    static unsigned int getHintCount();
    static void useOneHint();
    static void addHints(unsigned int to_add);
    static void saveGame();
    static void unlockAllCollectionsPurchased();
    static void buyAllCollectionsPurchased();
    static void buyCollection(const Collection::CollectionID id);
    static void disableAds();
private:
    friend class AppDelegate;
	friend class Loading;
#ifdef RW_DEBUG
public:
#endif
    static void init();
    static void release();
    static void readSavedData();

    static void readSettings();

    static void writeLevelsInformation(ADStreamOut&);
    static RW* _rw;
    RW();
    bool _expert_mode;
    bool _unlock_all_purchased;
    bool _buy_all_purchased;
    bool _ads_disabled;
    unsigned int _stamps_obtained;
    //unsigned int _stamps_max;
    unsigned int _crowns_obtained;
    CollectionsArr _collections;
    unsigned int _hints_count;
    static const uint16_t _levels_mark = 0xAABD;

    static void readLevelsInformation(ADStreamIn&);
    static void fillCollectionWithLevels(Collection*);
    static void parseSolutions(Level* l, const std::string& solutions);
#ifdef RW_BUILD_LEVELS
    static void prepareForLevelBuild();
#endif

private:
    static void updateStampsAndCrownsCount();
    static void saveLevelSolutions(Level* l);
    static void unlockNextLevel(Level* l, bool flush=true);
    //static void saveGame(ADStreamOut &os);

    static void flushCollectionInfo(Collection *a);
    static void readCollectionInfo(Collection* a);
    static void loadGame();
    //static const std::string _save_file_name;
    //static const std::string _settings_file_name;
//#ifndef CC_WIN8_METRO
//    static std::string _save_file_path;
//    static std::string _settings_file_path;
//    static std::string _poll_file_path;
//#else
    //static std::wstring _save_file_path;
    //static std::wstring _settings_file_path;
};

#endif // RW_H
