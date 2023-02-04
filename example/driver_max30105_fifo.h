/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_max30105_fifo.h
 * @brief     driver max30105 fifo header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-01-01
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/01/01  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MAX30105_FIFO_H
#define DRIVER_MAX30105_FIFO_H

#include "driver_max30105_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max30105_example_driver max30105 example driver function
 * @brief    max30105 example driver modules
 * @ingroup  max30105_driver
 * @{
 */

/**
 * @brief max30105 fifo example default definition
 */
#define MAX30105_FIFO_DEFAULT_SAMPLE_AVERAGING               MAX30105_SAMPLE_AVERAGING_8                         /**< sample 8 */
#define MAX30105_FIFO_DEFAULT_FIFO_ROLL                      MAX30105_BOOL_TRUE                                  /**< enable */
#define MAX30105_FIFO_DEFAULT_FIFO_ALMOST_FULL               0xF                                                 /**< 0xF */
#define MAX30105_FIFO_DEFAULT_MODE                           MAX30105_MODE_GREEN_RED_IR                          /**< green red ir mode */
#define MAX30105_FIFO_DEFAULT_PARTICLE_SENSING_ADC_RANGE     MAX30105_PARTICLE_SENSING_ADC_RANGE_4096            /**< adc range 4096 */
#define MAX30105_FIFO_DEFAULT_PARTICLE_SENSING_SAMPLE_RATE   MAX30105_PARTICLE_SENSING_SAMPLE_RATE_100_HZ        /**< 100 Hz */
#define MAX30105_FIFO_DEFAULT_ADC_RESOLUTION                 MAX30105_ADC_RESOLUTION_18_BIT                      /**< 18 bits */
#define MAX30105_FIFO_DEFAULT_LED_RED_PULSE_AMPLITUDE        0x7F                                                /**< 0x7F */
#define MAX30105_FIFO_DEFAULT_LED_IR_PULSE_AMPLITUDE         0x7F                                                /**< 0x7F */
#define MAX30105_FIFO_DEFAULT_LED_GREEN_PULSE_AMPLITUDE      0x7F                                                /**< 0x7F */
#define MAX30105_FIFO_DEFAULT_LED_PROXIMITY_PULSE_AMPLITUDE  0x7F                                                /**< 0x7F */
#define MAX30105_FIFO_DEFAULT_PROXIMITY_THRESHOLD            1023                                                /**< 1023 */
#define MAX30105_FIFO_DEFAULT_SLOT1                          MAX30105_LED_RED_LED1_PA                            /**< led red */
#define MAX30105_FIFO_DEFAULT_SLOT2                          MAX30105_LED_IR_LED2_PA                             /**< led ir */
#define MAX30105_FIFO_DEFAULT_SLOT3                          MAX30105_LED_GREEN_LED3_PA                          /**< led none */
#define MAX30105_FIFO_DEFAULT_SLOT4                          MAX30105_LED_NONE                                   /**< led none */
#define MAX30105_FIFO_DEFAULT_DIE_TEMPERATURE                MAX30105_BOOL_FALSE                                 /**< disable */
#define MAX30105_FIFO_DEFAULT_INTERRUPT_DATA_RDY_EN          MAX30105_BOOL_FALSE                                 /**< disable */
#define MAX30105_FIFO_DEFAULT_INTERRUPT_ALC_OVF_EN           MAX30105_BOOL_TRUE                                  /**< enable */
#define MAX30105_FIFO_DEFAULT_INTERRUPT_DIE_TEMP_RDY_EN      MAX30105_BOOL_TRUE                                  /**< enable */

/**
 * @brief  fifo example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max30105_fifo_irq_handler(void);

/**
 * @brief     fifo example init
 * @param[in] *fifo_receive_callback points to a fifo receive callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max30105_fifo_init(void (*fifo_receive_callback)(uint8_t type));

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max30105_fifo_deinit(void);

/**
 * @brief         read the data
 * @param[out]    *raw_red points to a red raw data buffer
 * @param[out]    *raw_ir points to a ir raw data buffer
 * @param[out]    *raw_green points to a green raw data buffer
 * @param[in,out] *len points to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          none
 */
uint8_t max30105_fifo_read(uint32_t *raw_red, uint32_t *raw_ir, uint32_t *raw_green, uint8_t *len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
