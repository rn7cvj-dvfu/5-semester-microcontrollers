#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "pwm.h"

#include "button.h"

void button_listener(void *params)
{
    // true - down, false - up

    void (*on_short_press)(void) = ((void (**)(void))params)[0];
    void (*on_long_press)(void) = ((void **)params)[1];

    int time = 0;

    bool prev_button_state = false;

    while (1)
    {
        uint32_t gpio_status = gpio_input_get() & BIT5;

        bool current_button_state = (gpio_status & BIT5) == 0;

        if (current_button_state && !prev_button_state)
        {
            time = 0;
        }

        if (!current_button_state && prev_button_state)
        {

            if (time < 1000)
            {
                on_short_press();
            }

            if (time >= 1000)
            {
                on_long_press();
            }

            time = 0;
        }

        vTaskDelay(50 / portTICK_RATE_MS);

        prev_button_state = current_button_state;
        time += 50;
    }
}
