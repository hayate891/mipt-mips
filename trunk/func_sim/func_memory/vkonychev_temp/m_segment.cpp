//My_Lib
#include <m_segment.h>
#include <m_page.h>
//uArch lib
#include <types.h>

//C++ lib
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

//////////////////////////////////////////////////////////////////////

Segment :: Segment ()
		{
			cout << "I need seg_size and page_size\n";
		}

//////////////////////////////////////////////////////////////////////

Segment :: Segment ( uint64 seg_bits, uint64 page_bits )
		{
			seg_size = pow ( 2, seg_bits );
			page_size =  pow ( 2, page_bits );

			Seg.resize( seg_size, NULL );
		}

//////////////////////////////////////////////////////////////////////

Segment :: Segment ( Segment &seg )
		{
			seg_size = seg.seg_size;
			page_size = seg.page_size;

			Seg.resize( seg_size, NULL );
			Seg = seg.Seg;	
		}

//////////////////////////////////////////////////////////////////////

Segment :: ~Segment ()
		{
		}


