#pragma once

#include <iostream>
#include <array>
#include <ctime>

namespace nsm {

    namespace LogColor {
        static const char* Reset        = "\33[0m";
        static const char* Bold         = "\33[1m";
        static const char* NoBold       = "\33[22m";
        static const char* Underline    = "\33[4m";
        static const char* NoUnderline  = "\33[24m";
        static const char* Invert       = "\33[7m";
        static const char* NoInvert     = "\33[27m";

        static const char* Black        = "\33[30m";
        static const char* Red          = "\33[31m";
        static const char* Green        = "\33[32m";
        static const char* Yellow       = "\33[33m";
        static const char* Blue         = "\33[34m";
        static const char* Magenta      = "\33[35m";
        static const char* Cyan         = "\33[36m";
        static const char* LightGray    = "\33[37m";
        static const char* Gray         = "\33[90m";
        static const char* LightRed     = "\33[91m";
        static const char* LightGreen   = "\33[92m";
        static const char* LightYellow  = "\33[93m";
        static const char* LightBlue    = "\33[94m";
        static const char* LightMagenta = "\33[95m";
        static const char* LightCyan    = "\33[96m";
        static const char* White        = "\33[97m";
    }

    namespace internal { 

        template <typename T>
        void print(T v) {
            std::cout << v;
        }

        template <typename T, typename... Args>
        void print(T v, Args... args) {
            std::cout << v;
            print(args...);
        }

        inline void timestamp() {
            std::array<char, 9> buffer = { 0 };
            const std::time_t time = std::time(nullptr);
            std::strftime(buffer.data(), buffer.size(), "%H:%M:%S", std::localtime(&time));
            std::cout << "[" << buffer.data() << "] ";
        }
    
    }


    template <typename... Args>
    void trace(Args...
        #ifndef NSM_DISABLE_LOG_TRACE
        args
        #endif
    ) {
        #ifndef NSM_DISABLE_LOG_TRACE
            internal::timestamp(); std::cout << LogColor::Gray <<
            #ifdef NSM_INTERNAL
                "[nsm/TRACE]: "
            #else
                "[main/TRACE]: "
            #endif
            ; internal::print(args...);
            std::cout << LogColor::Reset << std::endl;
        #endif
    }

    template <typename... Args>
    void info(Args...
        #ifndef NSM_DISABLE_LOG_INFO
        args
        #endif
    ) {
        #ifndef NSM_DISABLE_LOG_INFO
            internal::timestamp(); std::cout <<
            #ifdef NSM_INTERNAL
                "[nsm/INFO]: "
            #else
                "[main/INFO]: "
            #endif
            ; internal::print(args...);
            std::cout << std::endl;
        #endif
    }


    template <typename... Args>
    void warn(Args...
        #ifndef NSM_DISABLE_LOG_WARN
        args
        #endif
    ) {
        #ifndef NSM_DISABLE_LOG_WARN
            internal::timestamp(); std::cout << LogColor::Yellow <<
            #ifdef NSM_INTERNAL
                "[nsm/WARN]: "
            #else
                "[main/WARN]: "
            #endif
            ; internal::print(args...);
            std::cout << LogColor::Reset << std::endl;
        #endif
    }

    template <typename... Args>
    void error(Args... 
        #ifndef NSM_DISABLE_LOG_ERROR
        args
        #endif
    ) {
        #ifndef NSM_DISABLE_LOG_ERROR
            internal::timestamp(); std::cout << LogColor::Red <<
            #ifdef NSM_INTERNAL
                "[nsm/ERROR]: "
            #else
                "[main/ERROR]: "
            #endif
            ; internal::print(args...);
            std::cout << LogColor::Reset << std::endl;
        #endif
    }

}
