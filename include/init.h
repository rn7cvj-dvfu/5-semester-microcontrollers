
#include "esp_common.h"

#define UART_BAUD_RATE 9600

#define PWM_CHANNEL_COUNT 4

void init_uart();

void init_pwm();

uint32 user_rf_cal_sector_set(void);