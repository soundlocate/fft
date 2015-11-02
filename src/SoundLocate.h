#ifndef _SOUNDLOCATE
#define _SOUNDLOCATE

#include <vector>
#include "IStream.h"
#include "OStream.h"

class SoundLocate {
public:
	SoundLocate(IStream & in, OStream & out, int blocksize);
	int start();
private:
	int m_start();
	
    unsigned int m_blocksize;
	IStream m_in;
	OStream m_out;
};


#endif
