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
 * @file      driver_max30105.h
 * @brief     driver max30105 header file
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

#ifndef DRIVER_MAX30105_H
#define DRIVER_MAX30105_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max30105_driver max30105 driver function
 * @brief    max30105 driver modules
 * @{
 */

/**
 * @addtogroup max30105_basic_driver
 * @{
 */

/**
 * @brief max30105 bool enumeration definition
 */
typedef enum
{
    MAX30105_BOOL_FALSE = 0x00,        /**< false */
    MAX30105_BOOL_TRUE  = 0x01,        /**< true */
} max30105_bool_t;

/**
 * @brief max30105 sample averaging enumeration definition
 */
typedef enum
{
    MAX30105_SAMPLE_AVERAGING_1  = 0x00,        /**< no averaging */
    MAX30105_SAMPLE_AVERAGING_2  = 0x01,        /**< averaging 2 */
    MAX30105_SAMPLE_AVERAGING_4  = 0x02,        /**< averaging 4 */
    MAX30105_SAMPLE_AVERAGING_8  = 0x03,        /**< averaging 8 */
    MAX30105_SAMPLE_AVERAGING_16 = 0x04,        /**< averaging 16 */
    MAX30105_SAMPLE_AVERAGING_32 = 0x05,        /**< averaging 32 */
} max30105_sample_averaging_t;

/**
 * @brief max30105 mode enumeration definition
 */
typedef enum
{
    MAX30105_MODE_RED          = 0x02,        /**< red only mode */
    MAX30105_MODE_RED_IR       = 0x03,        /**< red and ir mode */
    MAX30105_MODE_GREEN_RED_IR = 0x07,        /**< green, red and ir mode */
} max30105_mode_t;

/**
 * @brief max30105 interrupt status enumeration definition
 */
typedef enum
{
    MAX30105_INTERRUPT_STATUS_FIFO_FULL    = 7,        /**< fifo almost full flag */
    MAX30105_INTERRUPT_STATUS_DATA_RDY     = 6,        /**< new fifo data ready */
    MAX30105_INTERRUPT_STATUS_ALC_OVF      = 5,        /**< ambient light cancellation overflow */
    MAX30105_INTERRUPT_STATUS_PROX_INT     = 4,        /**< proximity threshold triggered */
    MAX30105_INTERRUPT_STATUS_PWR_RDY      = 0,        /**< power ready flag */
    MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY = 1,        /**< internal temperature ready flag */
} max30105_interrupt_status_t;

/**
 * @brief max30105 interrupt enumeration definition
 */
typedef enum
{
    MAX30105_INTERRUPT_FIFO_FULL_EN    = 7,        /**< fifo almost full enable */
    MAX30105_INTERRUPT_DATA_RDY_EN     = 6,        /**< new fifo data ready enable */
    MAX30105_INTERRUPT_ALC_OVF_EN      = 5,        /**< ambient light cancellation overflow enable */
    MAX30105_INTERRUPT_PROX_INT_EN     = 4,        /**< proximity threshold interrupt enable */
    MAX30105_INTERRUPT_DIE_TEMP_RDY_EN = 1,        /**< internal temperature enable */
} max30105_interrupt_t;

/**
 * @brief max30105 particle sensing adc range enumeration definition
 */
typedef enum
{
    MAX30105_PARTICLE_SENSING_ADC_RANGE_2048  = 0,        /**< range 2048 */
    MAX30105_PARTICLE_SENSING_ADC_RANGE_4096  = 1,        /**< range 4096 */
    MAX30105_PARTICLE_SENSING_ADC_RANGE_8192  = 2,        /**< range 8192 */
    MAX30105_PARTICLE_SENSING_ADC_RANGE_16384 = 3,        /**< range 16384 */
} max30105_particle_sensing_adc_range_t;

/**
 * @brief max30105 particle sensing sample rate enumeration definition
 */
typedef enum
{
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_50_HZ   = 0,        /**< 50Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_100_HZ  = 1,        /**< 100Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_200_HZ  = 2,        /**< 200Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_400_HZ  = 3,        /**< 400Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_800_HZ  = 4,        /**< 800Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_1000_HZ = 5,        /**< 1000Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_1600_HZ = 6,        /**< 1600Hz */
    MAX30105_PARTICLE_SENSING_SAMPLE_RATE_3200_HZ = 7,        /**< 3200Hz */
} max30105_particle_sensing_sample_rate_t;

/**
 * @brief max30105 adc resolution enumeration definition
 */
typedef enum
{
    MAX30105_ADC_RESOLUTION_15_BIT = 0,        /**< 15 bits */
    MAX30105_ADC_RESOLUTION_16_BIT = 1,        /**< 16 bits */
    MAX30105_ADC_RESOLUTION_17_BIT = 2,        /**< 17 bits */
    MAX30105_ADC_RESOLUTION_18_BIT = 3,        /**< 18 bits */
} max30105_adc_resolution_t;

/**
 * @brief max30105 led enumeration definition
 */
typedef enum
{
    MAX30105_LED_NONE           = 0,        /**< time slot is disabled */
    MAX30105_LED_RED_LED1_PA    = 1,        /**< red led1 pa */
    MAX30105_LED_IR_LED2_PA     = 2,        /**< ir led2 pa */
    MAX30105_LED_GREEN_LED3_PA  = 3,        /**< green led3 pa */
    MAX30105_LED_RED_PILOT_PA   = 5,        /**< red pilot pa */
    MAX30105_LED_IR_PILOT_PA    = 6,        /**< ir pilot pa */
    MAX30105_LED_GREEN_PILOT_PA = 7,        /**< green pilot pa */
} max30105_led_t;

/**
 * @brief max30105 slot enumeration definition
 */
typedef enum
{
    MAX30105_SLOT_1 = 0,        /**< slot 1 */
    MAX30105_SLOT_2 = 1,        /**< slot 2 */
    MAX30105_SLOT_3 = 2,        /**< slot 3 */
    MAX30105_SLOT_4 = 3,        /**< slot 4 */
} max30105_slot_t;

/**
 * @brief max30105 handle structure definition
 */
typedef struct max30105_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t finished_flag;                                                              /**< finished flag */
    uint16_t raw;                                                                       /**< raw */
    float temperature;                                                                  /**< temperature */
    uint8_t buf[288];                                                                   /**< inner buffer */
} max30105_handle_t;

/**
 * @brief max30105 information structure definition
 */
typedef struct max30105_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} max30105_info_t;

/**
 * @}
 */

/**
 * @defgroup max30105_link_driver max30105 link driver function
 * @brief    max30105 link driver modules
 * @ingroup  max30105_driver
 * @{
 */

/**
 * @brief     initialize max30105_handle_t structure
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] STRUCTURE is max30105_handle_t
 * @note      none
 */
#define DRIVER_MAX30105_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to a max30105 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_MAX30105_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup max30105_basic_driver max30105 basic driver function
 * @brief    max30105 basic driver modules
 * @ingroup  max30105_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a max30105 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max30105_info(max30105_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_irq_handler(max30105_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 *            - 5 reset failed
 *            - 6 reset fifo failed
 * @note      none
 */
uint8_t max30105_init(max30105_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t max30105_deinit(max30105_handle_t *handle);

/**
 * @brief         read the data
 * @param[in]     *handle points to a max30105 handle structure
 * @param[out]    *raw_red points to a red raw data buffer
 * @param[out]    *raw_ir points to an ir raw data buffer
 * @param[out]    *raw_green points to a green raw data buffer
 * @param[in,out] *len points to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 fifo overrun
 *                - 5 mode is invalid
 * @note          none
 */
uint8_t max30105_read(max30105_handle_t *handle, uint32_t *raw_red, uint32_t *raw_ir, uint32_t *raw_green, uint8_t *len);

/**
 * @brief      read the temperature
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_read_temperature(max30105_handle_t *handle, uint16_t *raw, float *temp);

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  status is the interrupt status
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_interrupt_status(max30105_handle_t *handle, max30105_interrupt_status_t status, max30105_bool_t *enable);

/**
 * @brief     set the interrupt bool
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] type is the interrupt type
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_interrupt(max30105_handle_t *handle, max30105_interrupt_t type, max30105_bool_t enable);

/**
 * @brief      get the interrupt bool
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  type is the interrupt type
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_interrupt(max30105_handle_t *handle, max30105_interrupt_t type, max30105_bool_t *enable);

/**
 * @brief     set the fifo write pointer
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] pointer is the written pointer
 * @return    status code
 *            - 0 success
 *            - 1 set fifo write pointer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 pointer can't be over 0x1F
 * @note      pointer <= 0x1F
 */
uint8_t max30105_set_fifo_write_pointer(max30105_handle_t *handle, uint8_t pointer);

/**
 * @brief      get the fifo write pointer
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *pointer points to a pointer buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo write pointer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_write_pointer(max30105_handle_t *handle, uint8_t *pointer);

/**
 * @brief     set the fifo overflow counter
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] counter is the overflow counter
 * @return    status code
 *            - 0 success
 *            - 1 set fifo overflow counter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 counter can't be over 0x1F
 * @note      counter <= 0x1F
 */
uint8_t max30105_set_fifo_overflow_counter(max30105_handle_t *handle, uint8_t counter);

/**
 * @brief      get the fifo overflow counter
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *counter points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo overflow counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_overflow_counter(max30105_handle_t *handle, uint8_t *counter);

/**
 * @brief     set the fifo read pointer
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] pointer is the read pointer
 * @return    status code
 *            - 0 success
 *            - 1 set fifo read pointer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 pointer can't be over 0x1F
 * @note      pointer <= 0x1F
 */
uint8_t max30105_set_fifo_read_pointer(max30105_handle_t *handle, uint8_t pointer);

/**
 * @brief      get the fifo read pointer
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *pointer points to a pointer buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo read pointer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_read_pointer(max30105_handle_t *handle, uint8_t *pointer);

/**
 * @brief     set the fifo data
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] data is the fifo data
 * @return    status code
 *            - 0 success
 *            - 1 set fifo data failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_fifo_data(max30105_handle_t *handle, uint8_t data);

/**
 * @brief      get the fifo data
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *data points to a fifo data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_data(max30105_handle_t *handle, uint8_t *data);

/**
 * @brief     set the fifo sample averaging
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] sample is the fifo sample averaging
 * @return    status code
 *            - 0 success
 *            - 1 set fifo sample averaging failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_fifo_sample_averaging(max30105_handle_t *handle, max30105_sample_averaging_t sample);

/**
 * @brief      get the fifo sample averaging
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *sample points to a fifo sample averaging buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo sample averaging failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_sample_averaging(max30105_handle_t *handle, max30105_sample_averaging_t *sample);

/**
 * @brief     enable or disable the fifo roll 
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo roll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_fifo_roll(max30105_handle_t *handle, max30105_bool_t enable);

/**
 * @brief      get the fifo roll status
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo roll failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_roll(max30105_handle_t *handle, max30105_bool_t *enable);

/**
 * @brief     set the fifo almost full value
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] value is the fifo almost full value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo almost full failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 value can't be over 0xF
 * @note      none
 */
uint8_t max30105_set_fifo_almost_full(max30105_handle_t *handle, uint8_t value);

/**
 * @brief      get the fifo almost full value
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *value points to a fifo almost full value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo almost full failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_get_fifo_almost_full(max30105_handle_t *handle, uint8_t *value);

/**
 * @brief     set the shutdown
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set shutdown failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_shutdown(max30105_handle_t *handle, max30105_bool_t enable);

/**
 * @brief      get the shutdown
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shutdown failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_shutdown(max30105_handle_t *handle, max30105_bool_t *enable);

/**
 * @brief     reset the chip
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_reset(max30105_handle_t *handle);

/**
 * @brief     set the mode
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_mode(max30105_handle_t *handle, max30105_mode_t mode);

/**
 * @brief      get the mode
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_mode(max30105_handle_t *handle, max30105_mode_t *mode);

/**
 * @brief     set the particle sensing adc range
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] range is the particle sensing adc range
 * @return    status code
 *            - 0 success
 *            - 1 set particle sensing adc range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_particle_sensing_adc_range(max30105_handle_t *handle, max30105_particle_sensing_adc_range_t range);

/**
 * @brief      get the particle sensing adc range
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *range points to a particle sensing adc range buffer
 * @return     status code
 *             - 0 success
 *             - 1 get particle sensing adc range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_particle_sensing_adc_range(max30105_handle_t *handle, max30105_particle_sensing_adc_range_t *range);

/**
 * @brief     set the particle sensing sample rate
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] rate is the particle sensing sample rate
 * @return    status code
 *            - 0 success
 *            - 1 set particle sensing sample rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_particle_sensing_sample_rate(max30105_handle_t *handle, max30105_particle_sensing_sample_rate_t rate);

/**
 * @brief      get the particle sensing sample rate
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *rate points to a particle sensing sample rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get particle sensing sample rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_particle_sensing_sample_rate(max30105_handle_t *handle, max30105_particle_sensing_sample_rate_t *rate);

/**
 * @brief     set the adc resolution
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] resolution is the adc resolution
 * @return    status code
 *            - 0 success
 *            - 1 set adc resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_adc_resolution(max30105_handle_t *handle, max30105_adc_resolution_t resolution);

/**
 * @brief      get the adc resolution
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *resolution points to an adc resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get adc resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_adc_resolution(max30105_handle_t *handle, max30105_adc_resolution_t *resolution);

/**
 * @brief     set the red led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the red led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led red pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_red_pulse_amplitude(max30105_handle_t *handle, uint8_t amp);

/**
 * @brief      get the red led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to a red led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led red pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_red_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp);

/**
 * @brief     set the ir led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the ir led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led ir pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_ir_pulse_amplitude(max30105_handle_t *handle, uint8_t amp);

/**
 * @brief      get the ir led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to an ir led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led ir pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_ir_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp);

/**
 * @brief     set the green led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the green led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led green pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_green_pulse_amplitude(max30105_handle_t *handle, uint8_t amp);

/**
 * @brief      get the green led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to a green led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led green pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_green_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp);

/**
 * @brief     set the proximity led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the proximity led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led proximity pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_proximity_pulse_amplitude(max30105_handle_t *handle, uint8_t amp);

/**
 * @brief      get the proximity led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to a proximity led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led proximity pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_proximity_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp);

/**
 * @brief     set the led slot
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] slot is the slot number
 * @param[in] led is the led mode
 * @return    status code
 *            - 0 success
 *            - 1 set slot failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_slot(max30105_handle_t *handle, max30105_slot_t slot, max30105_led_t led);

/**
 * @brief      get the led slot
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  slot is the slot number
 * @param[out] *led points to a led mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get slot failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_slot(max30105_handle_t *handle, max30105_slot_t slot, max30105_led_t *led);

/**
 * @brief     enable or disable die temperature
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set die temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_die_temperature(max30105_handle_t *handle, max30105_bool_t enable);

/**
 * @brief      get the die temperature status
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get die temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_die_temperature(max30105_handle_t *handle, max30105_bool_t *enable);

/**
 * @brief     set the proximity interrupt threshold
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] threshold is the proximity interrupt threshold
 * @return    status code
 *            - 0 success
 *            - 1 set proximity interrupt threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_proximity_interrupt_threshold(max30105_handle_t *handle, uint8_t threshold);

/**
 * @brief      get the proximity interrupt threshold
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *threshold points to a proximity interrupt threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get proximity interrupt threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_proximity_interrupt_threshold(max30105_handle_t *handle, uint8_t *threshold);

/**
 * @brief      convert the proximity threshold to the register raw data
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  adc is the adc range
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_proximity_threshold_convert_to_register(max30105_handle_t *handle, uint32_t adc, uint8_t *reg);

/**
 * @brief      convert the register raw data to the proximity threshold
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *adc points to an adc range buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_proximity_threshold_convert_to_data(max30105_handle_t *handle, uint8_t reg, uint32_t *adc);

/**
 * @brief      get the chip id
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *revision_id points to a revision id buffer
 * @param[out] *part_id points to a part id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_id(max30105_handle_t *handle, uint8_t *revision_id, uint8_t *part_id);

/**
 * @}
 */

/**
 * @defgroup max30105_extern_driver max30105 extern driver function
 * @brief    max30105 extern driver modules
 * @ingroup  max30105_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_reg(max30105_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_reg(max30105_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
