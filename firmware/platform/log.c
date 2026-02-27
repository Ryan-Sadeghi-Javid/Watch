#include "log.h"
#include <stdio.h>
#include <string.h>

void Log_Init(void) {}

void Log_Print(const char* level, const char* fmt, ...) {
  (void)level; (void)fmt;
  // For simplicity, we just ignore the log level and print to a fixed buffer.
  char buf[128];
}