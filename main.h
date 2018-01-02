//	main.h

#include <iostream>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <unistd.h>

//--------------------------------------------------------------------------------------------------

#define DEF_PCONS_INCLUDE
#include "list_h.h"
#undef DEF_PCONS_INCLUDE

using namespace std;


#define DEF_PCONS_CLASS
#include "list_h.h"
#undef DEF_PCONS_CLASS


#define DEF_PCONS_TYPES
#include "list_h.h"
#undef DEF_PCONS_TYPES


#define DEF_PCONS_CONTENT
#include "list_h.h"
#undef DEF_PCONS_CONTENT

//--------------------------------------------------------------------------------------------------


//	main.h	:-|