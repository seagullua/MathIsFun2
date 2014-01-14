#include "DefineLevels.h"
#ifdef RW_BUILD_LEVELS
START_DEFINITION(define_mif, "Resources_storage/levels.ad", 10)

#include "col1.collection"
#include "col2.collection"
#include "col3.collection"
#include "col4.collection"
#include "col5.collection"
#include "col6.collection"

#include "col1000.collection"
#include "col2000.collection"
#include "col3000.collection"
#include "col4000.collection"

END_DEFINITION

START_DEFINITION(define_junior, "Resources_storage/levels_junior.ad", 6)
#include "junior1.collection"
#include "junior2.collection"
#include "junior3.collection"
#include "junior4.collection"
#include "junior5.collection"
#include "junior6.collection"
END_DEFINITION

#endif
