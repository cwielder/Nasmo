#pragma once

#include <nsm/common.h>
#include <nsm/debug/log.h>

#ifdef NSM_DEBUG
    #define NSM_ASSERT(exp, ...)                                                            \
        do {                                                                                \
            if (!(((void)0), exp)) {                                                        \
                nsm::error(NSM_FILENAME, "(", __LINE__, "): Assert failed! ", __VA_ARGS__ );\
                NSM_BREAKPOINT();                                                           \
            }                                                                               \
        } while (0)
    
#else
    #define NSM_ASSERT(exp, ...) do { ((void)(exp)); } while(0)
#endif
