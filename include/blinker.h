

#define PARALELL_MODE 0
#define SERIAL_MODE 1

/*
    * channel_id - PWM channel id
      channel_id = 0, 1, 2, 3

    * low_value - low value of duty cycle
      low_value = 0, 1, 2, 3, ... 1023

    * high_value - high value of duty cycle
      high_value = low_value + 1, low_value + 2 ... 1023

    * period - circle time low -> high -> low, in ms, only for paralell mode

    * delay - delay between each step, in ms , for both modes

    * step_count - number of steps in circle, for serial mode

    * step_mod - if step % step_count = step_mod - highValue else lowValue  for serial mode ,

*/
typedef struct BlinkerParams
{
  int32_t channelId;
  int32_t lowValue;
  int32_t highValue;
  int32_t period;
  int32_t delay;
  int32_t step_delay;
  int32_t step_count;
  int32_t step_mod;
  int32_t msc;
  int32_t mode;
} BlinkerParams;

typedef struct TotalBlinkerParams
{
  BlinkerParams *params;
  u32_t delay;
  u32_t count;

} TotalBlinkerParams;

void total_blinker(void *params);

void blinker(void *params);

int toogle_mode(int mode);

int toogle_step_delay(int step_delay);
