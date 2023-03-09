
#pragma once



enum class PACKET_IDS {
    MSG_TEST= 666,
    MSG_TEST_RES= 667,

};


#ifdef _WIN32 
    #include "win/t1.pb.h"
#endif 

#ifdef __linux__
    #include "linux/t1.pb.h"
#endif 