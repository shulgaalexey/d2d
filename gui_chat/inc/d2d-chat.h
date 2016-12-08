#ifndef __d2d_chat_H__
#define __d2d_chat_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "D2D_CHAT"


#define BUBBLE_TEXT_WIDTH 400
#define BUBBLE_MAIN_TEXT_STYLE "<font_size=40>%s</font_size>"
#define BUBBLE_SUB_TEXT_STYLE "<font_size=20 font_weight=Bold color=#454545>%s</font_size>"


#define ICON_DIR "/opt/usr/apps/$(packageName)/res/images"

// TODO Fix the logging

#define ERR(fmt, arg...) do { /*hts_log(D2D_LOG_SYSLOG, D2D_LOG_ERR, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg);*/ } while (0)
#define INFO(fmt, arg...) do { /*hts_log(D2D_LOG_SYSLOG, D2D_LOG_INFO, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg);*/ } while (0)
#define DBG(fmt, arg...) do { /*hts_log(D2D_LOG_SYSLOG, D2D_LOG_DBG, LOG_TAG, "%s:%s(%d)> "fmt, __MODULE__, __func__, __LINE__, ##arg);*/ } while (0)



#if !defined(PACKAGE)
#define PACKAGE "org.example.d2d-chat"
#endif

#endif /* __d2d_chat_H__ */
