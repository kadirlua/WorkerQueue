#ifndef WORKERQUEUE_EXPORT_H_
#define WORKERQUEUE_EXPORT_H_

/* Windows DLL export/import */
#if defined(_WIN32) && !defined(__GNUC__) && defined(WORKERQUEUE_COMPILE_DLL)
    #if WORKERQUEUE_DLL_EXPORT
        #define WORKERQUEUE_API __declspec(dllexport)
    #else
        #define WORKERQUEUET_API __declspec(dllimport)
    #endif    
#else    
    #if __GNUC__ >= 4
        #define WORKERQUEUE_API __attribute__ ((visibility ("default")))
    #else
        #define WORKERQUEUE_API
    #endif
#endif

#endif