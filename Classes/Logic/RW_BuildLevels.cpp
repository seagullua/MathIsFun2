#include "RW.h"
#include <cassert>
#include "Math/Equation.h"
#include <sstream>
RW* RW::_rw = 0;

#ifdef RW_BUILD_LEVELS
RW::RW(){}
#endif

void RW::prepareForLevelBuild()
{
    _rw = new RW();
}

ADStreamOut& operator<<(ADStreamOut& os, const cocos2d::ccColor3B& color)
{
    uint16_t r = color.r;
    uint16_t g = color.g;
    uint16_t b = color.b;

    os << r << g << b;
    return os;
}

/**
 * @brief RW::writeLevelsInformation
 * @param os
 * use during AD file creation
 */
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
