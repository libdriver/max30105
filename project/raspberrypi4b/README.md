### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(max30105 REQUIRED)
```

#### 2.4 INT Pin

INT pin need a 4.3KΩ resistor connect to 5V.

### 3. MAX30105

#### 3.1 Command Instruction

1. Show max30105 chip and driver information.

   ```shell
   max30105 (-i | --information)
   ```

2. Show max30105 help.

   ```shell
   max30105 (-h | --help)
   ```

3. Show max30105 pin connections of the current board.

   ```shell
   max30105 (-p | --port)
   ```

4. Run max30105 register test.

   ```shell
   max30105 (-t reg | --test=reg)
   ```

5. Run max30105 fifo test, num means test times.

   ```shell
   max30105 (-t fifo | --test=fifo) [--times=<num>]
   ```

6. Run max30105 fifo function, num means read times.

   ```shell
   max30105 (-e fifo | --example=fifo) [--times=<num>]
   ```

#### 3.2 Command Example

```shell
./max30105 -i

max30105: chip is Maxim Integrated MAX30105.
max30105: manufacturer is Maxim Integrated.
max30105: interface is IIC.
max30105: driver version is 1.0.
max30105: min supply voltage is 1.7V.
max30105: max supply voltage is 2.0V.
max30105: max current is 50.00mA.
max30105: max temperature is 85.0C.
max30105: min temperature is -40.0C.
```

```shell
./max30105 -p

max30105: SCL connected to GPIO3(BCM).
max30105: SDA connected to GPIO2(BCM).
max30105: INT connected to GPIO17(BCM).
```

```shell
./max30105 -t reg

max30105: chip is Maxim Integrated MAX30105.
max30105: manufacturer is Maxim Integrated.
max30105: interface is IIC.
max30105: driver version is 1.0.
max30105: min supply voltage is 1.7V.
max30105: max supply voltage is 2.0V.
max30105: max current is 50.00mA.
max30105: max temperature is 85.0C.
max30105: min temperature is -40.0C.
max30105: start register test.
max30105: max30105_set_interrupt/max30105_get_interrupt test.
max30105: enable fifo full.
max30105: check interrupt ok.
max30105: disable fifo full.
max30105: check interrupt ok.
max30105: enable data ready.
max30105: check interrupt ok.
max30105: disable data ready.
max30105: check interrupt ok.
max30105: enable alc ovf.
max30105: check interrupt ok.
max30105: disable alc ovf.
max30105: check interrupt ok.
max30105: enable proximity threshold interrupt.
max30105: check interrupt ok.
max30105: disable proximity threshold interrupt.
max30105: check interrupt ok.
max30105: enable die temp ready.
max30105: check interrupt ok.
max30105: disable die temp ready.
max30105: check interrupt ok.
max30105: max30105_set_fifo_write_pointer/max30105_get_fifo_write_pointer test.
max30105: set fifo write pointer 10.
max30105: max30105_set_fifo_overflow_counter/max30105_get_fifo_overflow_counter test.
max30105: set fifo overflow counter 5.
max30105: max30105_set_fifo_read_pointer/max30105_get_fifo_read_pointer test.
max30105: set fifo read pointer 3.
max30105: max30105_set_fifo_data/max30105_get_fifo_data test.
max30105: set fifo data 7.
max30105: max30105_set_fifo_sample_averaging/max30105_get_fifo_sample_averaging test.
max30105: set sample averaging 1.
max30105: check sample ok.
max30105: set sample averaging 2.
max30105: check sample ok.
max30105: set sample averaging 4.
max30105: check sample ok.
max30105: set sample averaging 8.
max30105: check sample ok.
max30105: set sample averaging 16.
max30105: check sample ok.
max30105: set sample averaging 32.
max30105: check sample ok.
max30105: max30105_set_fifo_roll/max30105_get_fifo_roll test.
max30105: enable fifo roll.
max30105: check roll ok.
max30105: disable fifo roll.
max30105: check roll ok.
max30105: max30105_set_fifo_almost_full/max30105_get_fifo_almost_full test.
max30105: set fifo almost full 8.
max30105: check fifo almost full ok.
max30105: max30105_set_shutdown/max30105_get_shutdown test.
max30105: enable shutdown.
max30105: check shutdown ok.
max30105: disable shutdown.
max30105: check shutdown ok.
max30105: max30105_set_mode/max30105_get_mode test.
max30105: set red mode.
max30105: check mode ok.
max30105: set red ir mode.
max30105: check mode ok.
max30105: set red ir green mode.
max30105: check mode ok.
max30105: max30105_set_particle_sensing_adc_range/max30105_get_particle_sensing_adc_range test.
max30105: set particle sensing adc range 2048.
max30105: check particle sensing adc range ok.
max30105: set particle sensing adc range 4096.
max30105: check particle sensing adc range ok.
max30105: set particle sensing adc range 8192.
max30105: check particle sensing adc range ok.
max30105: set particle sensing adc range 16384.
max30105: check particle sensing adc range ok.
max30105: max30105_set_particle_sensing_sample_rate/max30105_get_particle_sensing_sample_rate test.
max30105: set particle sensing sample rate 50Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 100Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 200Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 400Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 800Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 1000Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 1600Hz.
max30105: check particle sensing sample rate ok.
max30105: set particle sensing sample rate 3200Hz.
max30105: check particle sensing sample rate ok.
max30105: max30105_set_adc_resolution/max30105_get_adc_resolution test.
max30105: set adc resolution 15 bits.
max30105: check adc resolution ok.
max30105: set adc resolution 16 bits.
max30105: check adc resolution ok.
max30105: set adc resolution 17 bits.
max30105: check adc resolution ok.
max30105: set adc resolution 18 bits.
max30105: check adc resolution ok.
max30105: max30105_set_led_red_pulse_amplitude/max30105_get_led_red_pulse_amplitude test.
max30105: set led red pulse amplitude 255.
max30105: check led red pulse amplitude ok.
max30105: max30105_set_led_ir_pulse_amplitude/max30105_get_led_ir_pulse_amplitude test.
max30105: set led ir pulse amplitude 74.
max30105: check led ir pulse amplitude ok.
max30105: max30105_set_led_green_pulse_amplitude/max30105_get_led_green_pulse_amplitude test.
max30105: set led green pulse amplitude 236.
max30105: check led green pulse amplitude ok.
max30105: max30105_set_led_proximity_pulse_amplitude/max30105_get_led_proximity_pulse_amplitude test.
max30105: set led proximity pulse amplitude 41.
max30105: check led proximity pulse amplitude ok.
max30105: max30105_set_slot/max30105_get_slot test.
max30105: set slot1 led none.
max30105: check slot1 ok.
max30105: set slot1 red led1 pa.
max30105: check slot1 ok.
max30105: set slot1 ir led2 pa.
max30105: check slot1 ok.
max30105: set slot1 green led3 pa.
max30105: check slot1 ok.
max30105: set slot1 red pilot pa.
max30105: check slot1 ok.
max30105: set slot1 ir pilot pa.
max30105: check slot1 ok.
max30105: set slot1 green pilot pa.
max30105: check slot1 ok.
max30105: set slot2 led none.
max30105: check slot2 ok.
max30105: set slot2 red led1 pa.
max30105: check slot2 ok.
max30105: set slot2 ir led2 pa.
max30105: check slot2 ok.
max30105: set slot2 green led3 pa.
max30105: check slot2 ok.
max30105: set slot2 red pilot pa.
max30105: check slot2 ok.
max30105: set slot2 ir pilot pa.
max30105: check slot2 ok.
max30105: set slot2 green pilot pa.
max30105: check slot2 ok.
max30105: set slot3 led none.
max30105: check slot3 ok.
max30105: set slot3 red led1 pa.
max30105: check slot3 ok.
max30105: set slot3 ir led2 pa.
max30105: check slot3 ok.
max30105: set slot3 green led3 pa.
max30105: check slot3 ok.
max30105: set slot3 red pilot pa.
max30105: check slot3 ok.
max30105: set slot3 ir pilot pa.
max30105: check slot3 ok.
max30105: set slot3 green pilot pa.
max30105: check slot3 ok.
max30105: set slot4 led none.
max30105: check slot4 ok.
max30105: set slot4 red led1 pa.
max30105: check slot4 ok.
max30105: set slot4 ir led2 pa.
max30105: check slot4 ok.
max30105: set slot4 green led3 pa.
max30105: check slot4 ok.
max30105: set slot4 red pilot pa.
max30105: check slot4 ok.
max30105: set slot4 ir pilot pa.
max30105: check slot4 ok.
max30105: set slot4 green pilot pa.
max30105: check slot4 ok.
max30105: max30105_set_die_temperature/max30105_get_die_temperature test.
max30105: disable die temperature.
max30105: check die temperature ok.
max30105: enable die temperature.
max30105: check die temperature ok.
max30105: max30105_set_proximity_interrupt_threshold/max30105_get_proximity_interrupt_threshold test.
max30105: set proximity interrupt threshold 205.
max30105: max30105_proximity_threshold_convert_to_register/max30105_proximity_threshold_convert_to_data test.
max30105: adc is 190278.
max30105: check adc is 190278.
max30105: max30105_get_id test.
max30105: revision id is 0x06 part id is 0x15.
max30105: max30105_get_interrupt_status test.
max30105: interrupt status fifo full is 0.
max30105: interrupt status data ready is 0.
max30105: interrupt status alc ovf is 0.
max30105: interrupt status proximity threshold is 0.
max30105: interrupt status pwr ready is 0.
max30105: interrupt status die temp ready is 0.
max30105: max30105_reset test.
max30105: check reset ok.
max30105: finish register test.
```

```shell
./max30105 -t fifo --times=3

max30105: chip is Maxim Integrated MAX30105.
max30105: manufacturer is Maxim Integrated.
max30105: interface is IIC.
max30105: driver version is 1.0.
max30105: min supply voltage is 1.7V.
max30105: max supply voltage is 2.0V.
max30105: max current is 50.00mA.
max30105: max temperature is 85.0C.
max30105: min temperature is -40.0C.
max30105: start fifo test.
max30105: irq die temp rdy.
max30105: temperature is 29.8750C.
max30105: irq proximity threshold.
max30105: irq fifo full with 17.
max30105: irq fifo full with 17.
max30105: irq fifo full with 17.
max30105: finish fifo test.
```

```shell
./max30105 -e fifo --times=3

max30105: irq prox int.
max30105: irq fifo full with 17.
max30105: 1/3.
max30105: irq fifo full with 17.
max30105: 2/3.
max30105: irq fifo full with 17.
max30105: 3/3.
```

```shell
./max30105 -h

Usage:
  max30105 (-i | --information)
  max30105 (-h | --help)
  max30105 (-p | --port)
  max30105 (-t reg | --test=reg)
  max30105 (-t fifo | --test=fifo) [--times=<num>]
  max30105 (-e fifo | --example=fifo) [--times=<num>]

Options:
  -e <fifo>, --example=<fifo>    Run the driver example.
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
  -t <reg | fifo>, --test=<reg | fifo>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
```

