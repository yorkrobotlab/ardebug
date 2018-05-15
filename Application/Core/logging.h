#ifndef __LOGGING_H__
#define __LOGGING_H__

#define PRINTF_FUNC printf

#ifndef PRINTF_FUNC
//#ifdef MICROBLAZE
#include "xil_printf.h"
#define PRINTF_FUNC xil_printf
//#else
//#define PRINTF_FUNC printf
//#endif
#endif

//#define DEBUG_LOGGING

#if defined(DEBUG_LOGGING)
#define DEBUG_LOG(...) {PRINTF_FUNC("[%s (%4d)] Info:  ", __FILE__, __LINE__);PRINTF_FUNC(__VA_ARGS__);PRINTF_FUNC("\n\r");fflush(stdout);}
#define DEBUG_ERR(...) {PRINTF_FUNC("[%s (%4d)] Error: ", __FILE__, __LINE__);PRINTF_FUNC(__VA_ARGS__);PRINTF_FUNC("\n\r");fflush(stderr);}
#else
#define DEBUG_LOG(...)
#define DEBUG_ERR(...)
#endif

#define ASSERT_CORRECT_RETURN(val, expected, message) {if(val != expected){DEBUG_ERR(message);}}
#define ASSERT_CORRECT_RETURN_SUCCESS_MESSAGE(val, expected, errorMessage, successMessage) {u32 actual=val;if(actual==expected){DEBUG_LOG(successMessage);}else{DEBUG_ERR(errorMessage);DEBUG_ERR("\tExpected %lu but got %lu", expected, actual);}}

#endif
