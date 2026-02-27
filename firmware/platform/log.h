#pragma once
#include <stdarg.h>
#include <stdint.h>

void Log_Init(void);
void Log_Print(const char* level, const char* fmt, ...);

#define LOGI(...) Log_Print("INFO", __VA_ARGS__)
#define LOGW(...) Log_Print("WARN", __VA_ARGS__)
#define LOGE(...) Log_Print("ERR ", __VA_ARGS__)