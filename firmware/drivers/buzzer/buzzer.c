#include "buzzer.h"
#include "platform/board.h"

static uint32_t tim_clk_hz(TIM_HandleTypeDef* tim){
  (void)tim;
  return 84000000;
}

void buzzer_init(Buzzer* b, TIM_HandleTypeDef* tim, uint32_t channel){
  b->tim = tim;
  b->channel = channel;
  HAL_TIM_PWM_Start(b->tim, b->channel);
}

void buzzer_tone(Buzzer* b, uint32_t hz, uint8_t duty_percent, uint32_t duration_ms){
  if (hz == 0) return;

  uint32_t clk = tim_clk_hz(b->tim);
  uint32_t arr = (clk / hz) - 1;

  if (duty_percent > 100) duty_percent = 100;
  uint32_t ccr = (arr + 1) * duty_percent / 100;

  __HAL_TIM_SET_AUTORELOAD(b->tim, arr);
  __HAL_TIM_SET_COMPARE(b->tim, b->channel, ccr);
  __HAL_TIM_SET_COUNTER(b->tim, 0);

  HAL_TIM_PWM_Start(b->tim, b->channel);
  Board_DelayMs(duration_ms);
  buzzer_stop(b);
}

void buzzer_stop(Buzzer* b){
  __HAL_TIM_SET_COMPARE(b->tim, b->channel, 0);
  HAL_TIM_PWM_Stop(b->tim, b->channel);
}