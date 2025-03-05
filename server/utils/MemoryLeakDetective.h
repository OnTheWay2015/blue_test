
#pragma once






#ifdef USE_MONITORED_NEW

#define MONITORED_NEW(Tag,Type,...)				MonitoredNew<Type>(__FILE__, __LINE__,Tag,##__VA_ARGS__)
#define MONITORED_DELETE(Ptr)					MonitoredDelete(Ptr)
#define MONITORED_NEW_ARRAY(Tag,Type,Size,...)	MonitoredNewArray<Type>(Size,__FILE__, __LINE__,Tag,##__VA_ARGS__)
#define MONITORED_DELETE_ARRAY(Ptr)				MonitoredDeleteArray(Ptr)
#define MONITORED_MALLOC(Tag,Size)				MonitoredMalloc<void>(Size,__FILE__, __LINE__,Tag)
#define MONITORED_REALLOC(Tag,Ptr,Size)			MonitoredRealloc<void>(Ptr,Size,__FILE__, __LINE__,Tag)
#define MONITORED_FREE(Ptr)						MonitoredFree(Ptr)

#else



#define MONITORED_NEW(Tag,Type,...)				new Type(__VA_ARGS__)
#define MONITORED_DELETE(Ptr)					delete Ptr
#define MONITORED_NEW_ARRAY(Tag,Type,Size,...)	new Type[Size](__VA_ARGS__)
#define MONITORED_DELETE_ARRAY(Ptr)				delete[] Ptr
#define MONITORED_MALLOC(Tag,Size)				::malloc(Size)
#define MONITORED_REALLOC(Tag,Ptr,Size)			::realloc(Ptr,Size)
#define MONITORED_FREE(Ptr)						::free(Ptr)

#endif


#define SAFE_DELETE(p)       { if(p) { MONITORED_DELETE(p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { MONITORED_DELETE_ARRAY(p);   (p)=NULL; } }
