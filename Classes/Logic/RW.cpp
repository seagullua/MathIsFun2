#include "RW.h"
#include <cassert>
//#include <fstream>
#include <ADLib/Device/ADSoundManager.h>

#include "Math/Equation.h"
#include <sstream>

#ifndef RW_DEBUG

#include "cocos2d.h"
using namespace cocos2d;
#endif


#include <fstream>
#include <ctime>
static const uint32_t ADS_MARK = 0xAD001010;

RW* RW::_rw = 0;
#ifdef RW_BUILD_LEVELS
void RW::prepareForLevelBuild()
{
    _rw = new RW();
}
#endif
void RW::init()
{
    //Init can be called only once
    assert(_rw == 0);

    _rw = new RW();

#ifdef JUNIOR
    _rw->_last_hints_add = 0;
#endif
#ifdef RW_TEST_INPUT
    std::ifstream input("../../Resources/levels.ad", std::ios::in | std::ios::binary);
    ADStreamIn is(input);
    readLevelsInformation(is);
#endif

#ifndef RW_DEBUG
#ifndef CC_WIN8_METRO
    std::string s = CCFileUtils::sharedFileUtils()->getWritablePath();
    //CCLog(s.c_str());
    _save_file_path = s + _save_file_name;
    _settings_file_path = s + _settings_file_name;
    _poll_file_path = s + "poll.mark";
    CCLOG(_save_file_path.c_str());
#else
    _save_file_path = CCFileUtils::sharedFileUtils()->getWriteablePathW() + CCUtf8ToUnicode(_save_file_name.c_str());
    _settings_file_path = CCFileUtils::sharedFileUtils()->getWriteablePathW() + CCUtf8ToUnicode(_settings_file_name.c_str());
#endif
#endif
}

#ifndef CC_WIN8_METRO
bool fileExists(const char* name)
{
    FILE *fp = fopen(name, "r");
    bool bRet = false;

    if (fp)
    {
        bRet = true;
        fclose(fp);
    }

    return bRet;
}
#else
#include <Windows.h>
bool fileExists(const wchar_t* name)
{
    WIN32_FILE_ATTRIBUTE_DATA  fileInfo;
    return GetFileAttributesExW(name, GetFileExInfoStandard, &fileInfo);
}
#endif

const std::string RW::_save_file_name = "mif_save.ad";
const std::string RW::_settings_file_name = "mif_settings.ad";
#ifndef CC_WIN8_METRO
std::string RW::_save_file_path = "";
std::string RW::_settings_file_path = "";
std::string RW::_poll_file_path = "";
#else
std::wstring RW::_save_file_path = L"";
std::wstring RW::_settings_file_path = L"";
#endif

Level* RW::getNextLevel(Level* l)
{
    unsigned int next_id = l->getLevelID()+1;
    Collection* col = l->getLevelCollection();
    if(next_id < col->getLevels().size())
    {
        Level* next_level = col->getLevels()[next_id];
        return next_level;
    }
    return 0;
}
Level* RW::skipLevel(Level* l)
{
    unlockNextLevel(l, true);
    return getNextLevel(l);
}
bool RW::hasTranslationPollAlreadyShown()
{
#ifndef JUNIOR
    if(_rw)
    {
        return fileExists(_poll_file_path.c_str());
    }
    return false;
#else
    return true;
#endif
}
void RW::pollIsShown()
{
#ifndef JUNIOR
    if(_rw)
    {
        std::ofstream os(_poll_file_path.c_str());
        os << 1;
    }
#endif
}

bool RW::unlockCollection(Collection* col)
{
    if(_rw)
    {
        if(col->_state != Collection::Locked)
            return false;
        else
        {
            if(RW::allStampsObtained() >= col->stampsToUnlock())
            {
                col->_state = Collection::Unlocked;
                return true;
            }
        }
    }
    return false;
}

void RW::unlockNextLevel(Level* l, bool flush)
{
    if(_rw)
    {
        Level* next = getNextLevel(l);
        if(next && next->_state == Level::Locked)
        {

            next->_state = Level::NoStamps;
            if(flush)
                saveGame();
        }
    }
}
bool RW::isAdsDisabled()
{
#ifdef JUNIOR
    return true;
#else
    return _rw->_ads_disabled;
#endif
}

bool RW::registerSolution(Level* l, const Solution& s)
{
    if(_rw)
    {
        assert(l->hasSolution(s));

        if(l->isSolutionFound(s))
            return false;
        else
        {
            //Add solution
            l->_found_solutions.push_back(s);


            //Update stamps counts
            l->updateStampsStatus();
            l->getLevelCollection()->updateStampsAndCrownsCount();
            RW::updateStampsAndCrownsCount();

            //unlock next (we do not need to save to disk
            unlockNextLevel(l, false);

            //Write to disk that we found solution
            saveGame();
            return true;
        }
    }
    return false;
}



void RW::release()
{
    if(_rw)
    {
        //saveGame();
        //TODO: release all objects
    }
}

void RW::loadGame(ADStreamIn& is)
{
    if(_rw)
    {
        //Read the number of collections
        uint16_t mark_check = 0;
        is >> mark_check;

        if(mark_check == _levels_mark)
        {

            uint32_t collections_number = 0;
            is >> collections_number;


            //Read all collections
            for(unsigned int i=0; i<collections_number && is.isOK(); ++i)
            {
                uint32_t col_id = 0;
                is >> col_id;

                CollectionsArr::iterator col = _rw->_collections.find(col_id);

                //If the collection id is valid
                if(col == _rw->_collections.end())
                    is.setError();
                else
                {
                    Collection* a = col->second;
                    readCollectionInfo(a, is);
                    a->updateStampsAndCrownsCount();
                }
            }

        }
    }

}
bool RW::isExpertMode()
{
    if(_rw)
    {
        return _rw->_expert_mode;
    }
    return false;
}

void RW::setExpertMode(bool expert)
{
    if(_rw)
    {
        _rw->_expert_mode = expert;
    }
}

void RW::readCollectionInfo(Collection* a, ADStreamIn& is)
{
    if(_rw)
    {
        uint32_t status = uint32_t(Collection::InShop);
        is >> status;


        //Read the number of levels in collection
        uint16_t levels_number = 0;
        is >> levels_number;
        a->_state = Collection::CollectionState(status);

        //If number is valid
        if(levels_number <= a->_levels.size())
        {
            //Read all levels
            for(unsigned int i=0; i<levels_number && is.isOK(); ++i)
            {
                uint32_t lev_id = static_cast<uint32_t>(a->_levels.size());
                is >> lev_id;

                //If the level id is valid
                if(lev_id >= a->_levels.size())
                    is.setError();
                else
                {
                    Level* l = a->_levels[lev_id];
                    //If we read about this level it should be unlocked
                    l->_state = Level::NoStamps;

                    //Read the found solutions
                    Solutions sol;
                    is >> sol;

                    //Add only valid solutions
                    l->_found_solutions.reserve(sol.size());
                    for(unsigned int j=0; j<sol.size(); ++j)
                    {
                        if(l->hasSolution(sol[j]))
                            l->_found_solutions.push_back(sol[j]);
                    }

                    //Get new stamp status
                    l->updateStampsStatus();

                }
            }
        }
    }
}


void RW::flushCollectionInfo(Collection* a, ADStreamOut& os)
{
    if(_rw)
    {
        os << uint32_t(a->getCollectionID());

        //Calculate the number of opened levels
        uint16_t unlocked_levels = 0;
        for(unsigned int i=0; i<a->_levels.size(); ++i)
        {
            Level* l = a->_levels[i];
            if(l->getLevelState() != Level::Locked)
                unlocked_levels++;
        }

        os << uint32_t(a->getCollectionState());

        //Flush only opened levels
        os << unlocked_levels;
        for(unsigned int i=0; i<a->_levels.size(); ++i)
        {
            Level* l = a->_levels[i];
            if(l->getLevelState() != Level::Locked)
            {
                os << uint32_t(l->getLevelID()) << l->_found_solutions;
            }
        }
    }

}

void RW::saveGame(ADStreamOut& os)
{
    if(_rw)
    {
        //CCLog("Save game started");
        //		//Find the number of opened collections
        //		unsigned int unlocked_collections = 0;
        //		for(unsigned int i=0; i<_rw->_collections.size(); ++i)
        //		{
        //			Collection* a = _rw->_collections[i];
        //			if(a->getCollectionState() == Collection::Unlocked)
        //				unlocked_collections++;
        //		}
        os << uint16_t(_rw->_levels_mark);
        os << uint32_t(_rw->_collections.size());

        //Flush each opened collection
        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;

            //if(a->getCollectionState() == Collection::Unlocked)
            //{
            flushCollectionInfo(a, os);
            //}
        }
        //CCLog("Save game ended");
    }

}
void RW::saveGame()
{
    if(_rw)
    {
        flushSettings();
        std::stringstream ss(std::ios::out | std::ios::binary);
        ADStreamOut os(ss);


        saveGame(os);

        std::ofstream oss(_save_file_path.c_str(), std::ios::out | std::ios::binary);
        oss.write(ss.str().c_str(), ss.str().length());

    }
}
void RW::deletePersistentInfo()
{
    if(_rw)
    {
        //Delete all data
        if(fileExists(_save_file_path.c_str()))
        {
            std::ofstream oss(_save_file_path.c_str(), std::ios::out | std::ios::binary);
            oss.close();
        }

        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            if(a->_state == Collection::Unlocked)
                a->_state = Collection::Locked;
        }
        readSavedData();
        saveGame();
    }
}

void RW::readSavedData()
{
    if(_rw)
    {
#ifndef RW_DEBUG
        readSettings();

#ifdef JUNIOR
        bool emit_dayly_hints = false;
        uint64_t curtime = time(0);
        if(curtime > _rw->_last_hints_add)
        {
            const uint64_t day = 3600 * 24;
            if(curtime > _rw->_last_hints_add + day)
            {
                emit_dayly_hints = true;
            }
            else
            {
                time_t curt = curtime;
                time_t lastt = _rw->_last_hints_add;
                struct tm * cur_info = localtime ( &curt );
                unsigned int cur_day = cur_info->tm_mday;

                struct tm * last_info = localtime ( &lastt );
                unsigned int last_day = last_info->tm_mday;

                if(cur_day != last_day)
                    emit_dayly_hints = true;

            }
        }
        if(emit_dayly_hints)
        {
            _rw->_last_hints_add = curtime;
            _rw->addHints(3);
            _rw->flushSettings();
        }

#endif

        unsigned int stamps_max = 0;
        //Fill by the default info
        for(CollectionsArr::iterator it=_rw->_collections.begin();
            it!=_rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            //stamps_max += a->stampsMax();

            if(a->getCollectionID()==100)
                a->_state = Collection::Unlocked;
            if(a->getCollectionState() == Collection::InShop && _rw->_buy_all_purchased)
            {
                a->_state = Collection::Locked;
            }
            if(_rw->_unlock_all_purchased && a->getCollectionState() == Collection::Locked)
                a->_state = Collection::Unlocked;


            a->_stamps_obtained = 0;
            a->_crowns_obtained = 0;
            a->_max_stamps = 0;
            for(unsigned int j=0; j<a->_levels.size(); ++j)
            {
                Level* l = a->_levels[j];
                if(l->getLevelID() == 0)
                    l->_state = Level::NoStamps;
                else
                    l->_state = Level::Locked;
                l->_found_solutions.resize(0);
                a->_max_stamps += l->getSolutions().size();
            }
        }
        //_rw->_stamps_max = stamps_max;

        //Read saved info
        if(fileExists(_save_file_path.c_str()))
        {
            std::ifstream iss(_save_file_path.c_str(), std::ios::in | std::ios::binary);
            ADStreamIn is(iss);

            loadGame(is);
        }


        updateStampsAndCrownsCount();

#endif
    }
}
void RW::readSettings()
{
#ifndef RW_DEBUG
    if(_rw)
    {
        bool expert_mode = false;
        bool music_on = true;
        bool sounds_on = true;

        if(fileExists(_settings_file_path.c_str()))
        {
            std::ifstream iss(_settings_file_path.c_str(), std::ios::in | std::ios::binary);
            ADStreamIn is(iss);

            is >> expert_mode >> music_on >> sounds_on;

            uint16_t unlock_all = 0;
            uint16_t buy_all = 0;
            uint32_t hints = 0;
            is >> unlock_all >> buy_all >> hints;
            _rw->_unlock_all_purchased = (unlock_all == 1);
            _rw->_buy_all_purchased = (buy_all == 1);



#ifndef JUNIOR
            const unsigned int MAX_HINTS = 5000;
#else
            const unsigned int MAX_HINTS = 100;
#endif
            if(hints > MAX_HINTS)
                hints = MAX_HINTS;
            _rw->_hints_count = hints;



#ifdef JUNIOR
            is >> _rw->_last_hints_add;
#ifdef WIN32
            //   _rw->_hints_count += 1000;
#endif
#endif

            uint32_t ads_mark_check = 0;
            is >> ads_mark_check;

            if(ads_mark_check == ADS_MARK)
            {
                uint16_t ads_off = 0;
                is >> ads_off;

                _rw->_ads_disabled = (ads_off == 1);
            }

            if(!_rw->_ads_disabled)
            {
                if(_rw->_unlock_all_purchased || _rw->_buy_all_purchased)
                {
                    _rw->_ads_disabled = true;
                }
            }
        }

        RW::setExpertMode(expert_mode);
        if(music_on)
            ADSoundManager::turnOnMusic();
        else
            ADSoundManager::turnOffMusic();

        if(sounds_on)
            ADSoundManager::turnOnSound();
        else
            ADSoundManager::turnOffSound();
    }
#endif
}

void RW::flushSettings()
{
#ifndef RW_DEBUG
    if(_rw)
    {
        std::ofstream oss(_settings_file_path.c_str(), std::ios::out | std::ios::binary);
        ADStreamOut os(oss);

        bool expert_mode = RW::isExpertMode();
        bool music_on = ADSoundManager::isMusicTurnedOn();
        bool sounds_on = ADSoundManager::isSoundTurnedOn();

        os << expert_mode << music_on << sounds_on;
        os << uint16_t(_rw->_unlock_all_purchased ? 1 : 0) << uint16_t(_rw->_buy_all_purchased ? 1 : 0);
        os << uint32_t(_rw->_hints_count);


#ifdef JUNIOR
        os << _rw->_last_hints_add;
#endif

        os << ADS_MARK << uint16_t(_rw->_ads_disabled ? 1 : 0);

    }
#endif
}

const CollectionsArr &RW::getCollections()
{
    assert(_rw);
    return _rw->_collections;
}

unsigned int RW::allStampsObtained()
{
    assert(_rw);
    return _rw->_stamps_obtained;
}

//unsigned int RW::allStampsMax()
//{
//    assert(_rw);
//    return _rw->_stamps_max;
//}

unsigned int RW::allCrownsObtained()
{
    assert(_rw);
    return _rw->_crowns_obtained;
}

ADStreamOut& operator<<(ADStreamOut& os, const cocos2d::ccColor3B& color)
{
    uint16_t r = color.r;
    uint16_t g = color.g;
    uint16_t b = color.b;

    os << r << g << b;
    return os;
}

ADStreamIn& operator>>(ADStreamIn& is, cocos2d::ccColor3B& color)
{
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;
    is >> r >> g >> b;
    typedef unsigned char Type;
    color = ccc3((Type)r,(Type)g,(Type)b);
    return is;
}

void RW::writeLevelsInformation(ADStreamOut& os)
{
    if(_rw)
    {
        uint32_t coll_size = static_cast<uint32_t>(_rw->_collections.size());
        os << _rw->_levels_mark << coll_size;

        for(CollectionsArr::iterator it=_rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            uint16_t free = a->_collection_free ? 1 : 0;
            uint16_t difficulty = a->_collection_difficulty;
            os << uint32_t(a->_collection_id) << free << difficulty << a->_collection_color << a->_stamps_to_unlock;

            uint32_t level_number = static_cast<uint32_t>(a->_levels.size());
            os << level_number;

            for(unsigned int lev_id=0; lev_id<level_number; ++lev_id)
            {
                Level* l = a->_levels[lev_id];
                //std::string test =l->_equation->getUntouchedLhsString();
                os << l->_equation->getUntouchedLhsString() << l->_equation->getUntouchedRhsString();
                os << l->_existing_solutions;
            }
        }
    }
}

void RW::readLevelsInformation(ADStreamIn& is)
{
    if(_rw)
    {

        unsigned short int control_mark = 0;
        is >> control_mark;
        if(control_mark != _levels_mark)
            is.setError();
        else
        {
            unsigned int collection_size = 0;
            is >> collection_size;


            //_rw->_collections.resize(0);
            //_rw->_collections.reserve(collection_size);

            for(unsigned int col_id=0;
                col_id < collection_size && is.isOK();
                ++col_id)
            {
                Collection* a = new Collection;


                uint32_t id = 0;
                is >> id;
                a->_collection_id = id;
                _rw->_collections[id] = a;

                uint16_t free = 0;
                is >> free;
                a->_collection_free = (free == 1);

                if(a->_collection_free)
                    a->_state = Collection::Locked;
                else
                    a->_state = Collection::InShop;

                uint16_t difficulty = Collection::Easy;
                is >> difficulty;

                a->_collection_difficulty = Collection::Difficulty(difficulty);

                is >> a->_collection_color >> a->_stamps_to_unlock;



                unsigned int levels_number = 0;
                is >> levels_number;

                a->_levels.resize(0);
                a->_levels.reserve(levels_number);

                for(unsigned int lev_id=0; lev_id < levels_number; ++lev_id)
                {
                    Level* l = new Level;
                    a->_levels.push_back(l);
                    l->_parent = a;
                    l->_level_id = lev_id;

                    std::string lhs="";
                    std::string rhs="";
                    is >> lhs >> rhs >> l->_existing_solutions;

                    l->_equation = new Equation(lhs,rhs);
                }

            }

        }

    }
}
void RW::updateStampsAndCrownsCount()
{
    if(_rw)
    {
        unsigned int stamps = 0;
        unsigned int crowns = 0;

        for(CollectionsArr::iterator it=_rw->_collections.begin();
            it!=_rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            stamps += a->stampsObtained();
            crowns += a->crownsObtained();
        }
        _rw->_crowns_obtained = crowns;
        _rw->_stamps_obtained = stamps;
    }
}
unsigned int RW::getHintCount()
{
    return _rw->_hints_count;
}
void RW::addHints(unsigned int to_add)
{
    _rw->_hints_count += to_add;
    flushSettings();
}
void RW::unlockAllCollectionsPurchased()
{
    if(_rw)
    {
        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            if(a->getCollectionState() == Collection::Locked)
            {
                a->_state = Collection::Unlocked;
            }
        }
        _rw->_unlock_all_purchased = true;

        saveGame();

    }
}
void RW::buyAllCollectionsPurchased()
{
    if(_rw)
    {
        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            if(a->getCollectionState() == Collection::InShop)
            {
                if(_rw->_unlock_all_purchased)
                    a->_state = Collection::Unlocked;
                else
                    a->_state = Collection::Locked;

            }
        }
        _rw->_buy_all_purchased = true;

        saveGame();

    }
}
void RW::disableAds()
{
    _rw->_ads_disabled = true;
    flushSettings();
}

void RW::useOneHint()
{
    assert(_rw->_hints_count > 0);
    if(_rw->_hints_count > 0)
    {
        _rw->_hints_count--;
        flushSettings();
    }
}

void RW::buyCollection(const Collection::CollectionID id)
{
    if(_rw)
    {
        CollectionsArr::iterator it = _rw->_collections.find(id);
        if(it != _rw->_collections.end())
        {
            Collection* a = it->second;
            a->_state = Collection::Unlocked;
            saveGame();
        }
    }
}

RW::RW()
    : _expert_mode(false),
      #ifndef JUNIOR
      _hints_count(5),
      #else
      _hints_count(2),
      #endif
      _unlock_all_purchased(false),
      _buy_all_purchased(false),
      _ads_disabled(false)
{
}
