#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "pwm.h"

#include <freertos/task.h>

#include "init.h"
#include "blinker.h"
#include "button.h"

struct BlinkerParams blinker_params[4];

struct TotalBlinkerParams total_blinker_params;

void (*button_functions[])() = {NULL, NULL};

void handle_param(BlinkerParams *p)
{
    if (p->mode == PARALELL_MODE)
    {
        p->channelId = (p->channelId + 1) % PWM_CHANNEL_COUNT;
    }

    if (p->mode == SERIAL_MODE)
    {
        p->step_delay = toogle_step_delay(p->step_delay);
    }

    p->msc = 0;
}

void on_short_press()
{
    handle_param(blinker_params);
    handle_param(blinker_params + 1);
    handle_param(blinker_params + 2);
    handle_param(blinker_params + 3);
}

void on_long_press()
{

    blinker_params[0].mode = toogle_mode(blinker_params[0].mode);
    blinker_params[1].mode = toogle_mode(blinker_params[1].mode);
    blinker_params[2].mode = toogle_mode(blinker_params[2].mode);
    blinker_params[3].mode = toogle_mode(blinker_params[3].mode);

    // blinker_params[0].channelId = 0;
    // blinker_params[1].channelId = 1;
    // blinker_params[2].channelId = 2;
    // blinker_params[3].channelId = 3;

    // blinker_params[0].msc = 0;
    // blinker_params[1].msc = 0;
    // blinker_params[2].msc = 0;
    // blinker_params[3].msc = 0;
}

void user_init(void)
{
    init_uart();
    init_pwm();

    GPIO_AS_INPUT(GPIO_Pin_5);

    button_functions[0] = on_short_press;
    button_functions[1] = on_long_press;

    blinker_params[0].channelId = 0;
    blinker_params[0].lowValue = 0;
    blinker_params[0].highValue = 1023;
    blinker_params[0].period = 2000;
    blinker_params[0].delay = 1000;
    blinker_params[0].msc = 0;
    blinker_params[0].mode = PARALELL_MODE;
    blinker_params[0].step_delay = 1000;
    blinker_params[0].step_count = 4;
    blinker_params[0].step_mod = 0;

    blinker_params[1].channelId = 1;
    blinker_params[1].lowValue = 0;
    blinker_params[1].highValue = 1023;
    blinker_params[1].period = 4000;
    blinker_params[1].delay = 2000;
    blinker_params[1].msc = 0;
    blinker_params[1].mode = PARALELL_MODE;
    blinker_params[1].step_delay = 1000;
    blinker_params[1].step_count = 4;
    blinker_params[1].step_mod = 1;

    blinker_params[2].channelId = 2;
    blinker_params[2].lowValue = 0;
    blinker_params[2].highValue = 1023;
    blinker_params[2].period = 1000;
    blinker_params[2].delay = 10;
    blinker_params[2].msc = 0;
    blinker_params[2].mode = PARALELL_MODE;
    blinker_params[2].step_delay = 1000;
    blinker_params[2].step_count = 4;
    blinker_params[2].step_mod = 2;

    blinker_params[3].channelId = 3;
    blinker_params[3].lowValue = 0;
    blinker_params[3].highValue = 1023;
    blinker_params[3].period = 500;
    blinker_params[3].delay = 10;
    blinker_params[3].msc = 0;
    blinker_params[3].mode = PARALELL_MODE;
    blinker_params[3].step_delay = 1000;
    blinker_params[3].step_count = 4;
    blinker_params[3].step_mod = 3;

    // xTaskCreate(&blinker, "blink_task_1", 2048, blinker_params, 5, NULL);
    // xTaskCreate(&blinker, "blink_task_2", 2048, blinker_params + 1, 5, NULL);
    // xTaskCreate(&blinker, "blink_task_2", 2048, blinker_params + 2, 5, NULL);
    // xTaskCreate(&blinker, "blink_task_2", 2048, blinker_params + 3, 5, NULL);

    total_blinker_params.params = blinker_params;
    total_blinker_params.delay = 50;
    total_blinker_params.count = 4;

    xTaskCreate(&total_blinker, "total_blinker", 2048, &total_blinker_params, 5, NULL);

    xTaskCreate(&button_listener, "button_listener", 2048, button_functions, 5, NULL);
}
