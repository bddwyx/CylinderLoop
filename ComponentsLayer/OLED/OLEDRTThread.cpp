#include "OLED.h"
#include "rtthread.h"

static rt_thread_t OLEDThread = RT_NULL;

/**
 * @brief Thread entry of OLED refresh
 * @param para No use
 * @note Multiscreen Application not supported
 * @return (None)
 */
void OLEDThreadEntry(void* para){
    while(1){
        OLED::_oled_device.OLEDRefresh();
        rt_thread_delay(5);
    }
}

void BarThreadEntry(void* para){
    static uint8_t progress = 0;
    while(1){
        OLED::_oled_device.DrawProgressBar(13, 50, 100, 10, progress);
        progress = ++progress % 100;
        rt_thread_delay(40);
    }
}

static rt_thread_t BarThread = RT_NULL;

/**
 * @brief Start the OLED refresh thread
 * @return (None)
 */
void OLED::RTThreadInit() {
    constexpr auto entry = &OLEDThreadEntry;
    OLEDThread = rt_thread_create("OLED",
                                  entry,
                                  RT_NULL,
                                  512,
                                  3,
                                  2);
    rt_thread_startup(OLEDThread);

    BarThread = rt_thread_create("Bar",
                                 BarThreadEntry,
                                 RT_NULL,
                                 512,
                                 3,
                                 20);
    rt_thread_startup(BarThread);
}