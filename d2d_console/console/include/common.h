#if !defined(_D2D_CONV_CONSOLE_COMMON_H_)
#define _D2D_CONV_CONSOLE_COMMON_H_

// Log methods
// USE_DLOG_DEBUG: You can check logs by running 'dlogutil'
// USE_SYSLOG_DEBUG: You can check logs by reading '/var/log/messages'
// USE_FILE_DEBUG: You can check logs by reading '/var/log/messages'
#define USE_DLOG_DEBUG

#ifdef __cplusplus
extern "C"
{
#endif


enum hts_log_type {
	D2D_LOG_PRINT_FILE	= 1,
	D2D_LOG_SYSLOG		= 2,
	D2D_LOG_DLOG		= 3,
};

enum hts_priority_type {
	D2D_LOG_ERR		= 1,
	D2D_LOG_DBG		= 2,
	D2D_LOG_INFO		= 3,
};

void hts_log(int type, int priority, const char *tag, const char *fmt, ...);


#define MICROSECONDS(tv)        ((tv.tv_sec * 1000000ll) + tv.tv_usec)

#ifndef __MODULE__
#include <string.h>
#define __MODULE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

//for new log system - dlog
#ifdef LOG_TAG
	#undef LOG_TAG
#endif
#define LOG_TAG "D2D_CONV_CONSOLE"
//


#if defined(_DEBUG) || defined(USE_FILE_DEBUG)

#define DbgPrint(fmt, arg...)   do { hts_log(D2D_LOG_PRINT_FILE, 0, LOG_TAG, "[D2D_MSG_PRT][%s:%d] "fmt"\n", __MODULE__, __LINE__, ##arg); } while (0)

#endif

#if defined(USE_SYSLOG_DEBUG)

#define ERR(fmt, arg...) do { hts_log(D2D_LOG_SYSLOG, D2D_LOG_ERR, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)
#define INFO(fmt, arg...) do { hts_log(D2D_LOG_SYSLOG, D2D_LOG_INFO, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)
#define DBG(fmt, arg...) do { hts_log(D2D_LOG_SYSLOG, D2D_LOG_DBG, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)


#elif defined(_DEBUG) || defined(USE_DLOG_DEBUG)

#define ERR(fmt, arg...) do { hts_log(D2D_LOG_DLOG, D2D_LOG_ERR, LOG_TAG, "%s:%s(%d)> " fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)
#define INFO(fmt, arg...) do { hts_log(D2D_LOG_DLOG, D2D_LOG_INFO, LOG_TAG, "%s:%s(%d)> " fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)
#define DBG(fmt, arg...) do { hts_log(D2D_LOG_DLOG, D2D_LOG_DBG, LOG_TAG, "%s:%s(%d)> " fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)

#elif defined(USE_FILE_DEBUG)

#define ERR(fmt, arg...)	do { hts_log(D2D_LOG_PRINT_FILE, 0, LOG_TAG, "[D2D_MSG_ERR][%s:%d] "fmt"\n", __MODULE__, __LINE__, ##arg); } while (0)
#define DBG(fmt, arg...)	do { hts_log(D2D_LOG_PRINT_FILE, 0, LOG_TAG, "[D2D_MSG_DBG][%s:%d] "fmt"\n", __MODULE__, __LINE__, ##arg); } while (0)
#define INFO(fmt, arg...)	do { hts_log(D2D_LOG_PRINT_FILE, 0, LOG_TAG, "[D2D_MSG_INFO][%s:%d] "fmt"\n", __MODULE__, __LINE__, ##arg); } while (0)

#elif defined(USE_DLOG_DEBUG)

#define ERR(fmt, arg...) do { hts_log(D2D_LOG_DLOG, D2D_LOG_ERR, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)
#define INFO(fmt, arg...) do { hts_log(D2D_LOG_DLOG, D2D_LOG_INFO, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg); } while (0)

#define DBG(...)
#define DbgPrint(...)

#else
#define ERR(fmt, arg...) do { hts_log(D2D_LOG_DLOG, D2D_LOG_ERR, LOG_TAG, fmt, ##arg); } while (0)

#define DbgPrint(...)
#define DBG(...)
#define INFO(...)
#endif


#if defined(_DEBUG)
#  define warn_if(expr, fmt, arg...) do { \
		if(expr) { \
			DBG("(%s) -> "fmt, #expr, ##arg); \
		} \
	} while (0)
#  define ret_if(expr) do { \
		if(expr) { \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return; \
		} \
	} while (0)
#  define retv_if(expr, val) do { \
		if(expr) { \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return (val); \
		} \
	} while (0)
#  define retm_if(expr, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return; \
		} \
	} while (0)
#  define retvm_if(expr, val, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return (val); \
		} \
	} while (0)

#else
#  define warn_if(expr, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
		} \
	} while (0)
#  define ret_if(expr) do { \
		if(expr) { \
			return; \
		} \
	} while (0)
#  define retv_if(expr, val) do { \
		if(expr) { \
			return (val); \
		} \
	} while (0)
#  define retm_if(expr, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			return; \
		} \
	} while (0)
#  define retvm_if(expr, val, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			return (val); \
		} \
	} while (0)

#endif


#ifdef __cplusplus
}
#endif

#endif
//! End of a file
