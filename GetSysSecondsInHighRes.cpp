#include <windows.h>    //for include Winbase.h

//return the time in second that the current system run in high-resolution 
double GetSysSecondsInHighRes()
{
    static LARGE_INTERGER static_performance_freq;
    LARGE_INTERGER performance_count;
    double time_in_second;
    if(static_performance_freq.QuadPart == 0)
    {
        if(!QueryPerformanceFrequency(&static_performance_freq))
        {
            return 0;
        }
    }
    if(!QueryPerformanceCounter(&performance_count))
    {
        return 0;
    }
    time_in_second = (double)performance_count.QuardPart / static_performance_freq.QuadPart;
    return time_in_second;
}

