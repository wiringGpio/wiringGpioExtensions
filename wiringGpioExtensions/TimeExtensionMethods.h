#pragma once
#include <sys/time.h>


//  Returns the unix time in milliseconds
inline unsigned long long GetUnixTimeMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, 0x00);
			
	return ( (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000);
}