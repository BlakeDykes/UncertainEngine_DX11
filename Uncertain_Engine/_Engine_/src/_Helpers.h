#pragma once

#include "AudioManager.h"

#ifdef DBOUT
#define PRINTCOMMAND( comDescriptor, id1, objectID )	\
{								\
	std::ostringstream os_;		\
	os_ << comDescriptor << ": " << id1 << " | GameObject ID: " << objectID << "\n";	\
	OutputDebugString(os_.str().c_str()); \
}
#else
#define PRINTCOMMAND(comDescriptor, id1, objectID)	{}
#endif