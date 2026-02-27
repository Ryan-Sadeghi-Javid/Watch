#include "platform/board.h"
#include "platform/log.h"
#include "app/app.h"

int main(void) {
  Board_Init();
  Log_Init();

  LOGI("Boot");

  App_Init();

  while (1) {
    App_Tick();
  }
}