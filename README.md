# codal-libopencm3
Codal framework ported to libopencm3 to support MakeCode on STM32 Blue Pill

Based on code from:

https://github.com/lupyuen/send_altitude_cocoos

https://github.com/lancaster-university/codal-stm32

https://github.com/mmoskal/codal-generic-f103re

## PlatformIO Build Log

```
> Executing task in folder codal-libopencm3: platformio run <

Processing bluepill_f103c8 (framework: libopencm3; platform: ststm32; board: bluepill_f103c8)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/ststm32/bluepill_f103c8.html
PLATFORM: ST STM32 > BluePill F103C8
HARDWARE: STM32F103C8T6 72MHz 20KB RAM (64KB Flash)
DEBUG: CURRENT(blackmagic) EXTERNAL(blackmagic, jlink, stlink)
Library Dependency Finder -> http://bit.ly/configure-pio-ldf
LDF MODES: FINDER(chain+) COMPATIBILITY(off)
Collected 25 compatible libraries
Scanning dependencies...
Dependency Graph
|-- <boost_iterator>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |-- <boost_preprocessor>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|-- <boost_lockfree>
|   |-- <boost_iterator>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |-- <boost_parameter>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_predef>
|   |-- <boost_preprocessor>
|-- <logger>
|-- <boost_config>
|-- <bluepill>
|   |-- <cm>
|   |   |-- <logger>
|   |-- <logger>
|   |-- <codal-core>
|   |   |-- <cm>
|   |   |   |-- <logger>
|-- <simulator>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |-- <logger>
|   |-- <spiint>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|-- <boost_parameter>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_preprocessor>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|-- <boost_preprocessor>
|-- <uartint>
|   |-- <boost_lockfree>
|   |   |-- <boost_iterator>
|   |   |   |-- <boost_mpl>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_utility>
|   |   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_core>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_assert>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_predef>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_detail>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |-- <boost_parameter>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_mpl>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_utility>
|   |   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_core>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_assert>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_predef>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_detail>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |   |-- <boost_preprocessor>
|   |-- <logger>
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|-- <boost_predef>
|-- <boost_type_traits>
|   |-- <boost_config>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|-- <codal-core>
|   |-- <cm>
|   |   |-- <logger>
|-- <boost_core>
|   |-- <boost_config>
|   |-- <boost_assert>
|-- <cocoOS> 5.0.4
|-- <i2cint>
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |-- <logger>
|-- <spiint>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |-- <logger>
|-- <boost_detail>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_preprocessor>
|   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|-- <porting>
|   |-- <cocoOS> 5.0.4
|   |-- <i2cint>
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <spiint>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|-- <BME280> 2.3.0
|   |-- <porting>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <i2cint>
|   |   |   |-- <bluepill>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |   |   |   |-- <cm>
|   |   |   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |-- <spiint>
|   |   |   |-- <cocoOS> 5.0.4
|   |   |   |-- <bluepill>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |   |   |   |-- <cm>
|   |   |   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |-- <cocoOS> 5.0.4
|   |-- <spiint>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <i2cint>
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <codal-core>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |-- <logger>
|   |-- <cm>
|   |   |-- <logger>
|-- <boost_assert>
|-- <boost_mpl>
|   |-- <boost_preprocessor>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_predef>
|-- <boost_static_assert>
|   |-- <boost_config>
|-- <cm>
|   |-- <logger>
|-- <boost_utility>
|   |-- <boost_preprocessor>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
Compiling .pioenvs/bluepill_f103c8/src/aggregate.o
Compiling .pioenvs/bluepill_f103c8/src/alt_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/bme280.o
Compiling .pioenvs/bluepill_f103c8/src/display.o
Compiling .pioenvs/bluepill_f103c8/src/downlink.o
Compiling .pioenvs/bluepill_f103c8/src/gyro_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/humid_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/main.o
Compiling .pioenvs/bluepill_f103c8/src/sensor.o
Compiling .pioenvs/bluepill_f103c8/src/temp_event_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/temp_sensor.o
src/sensor.cpp:213:13: warning: 'bool is_valid_event_sensor(Sensor*)' defined but not used [-Wunused-function]
static bool is_valid_event_sensor(Sensor *sensor) {
^~~~~~~~~~~~~~~~~~~~~
Compiling .pioenvs/bluepill_f103c8/src/uart.o
Compiling .pioenvs/bluepill_f103c8/src/wisol.o
Compiling .pioenvs/bluepill_f103c8/src/wstring.o
Archiving .pioenvs/bluepill_f103c8/libd23/libboost_preprocessor.a
Archiving .pioenvs/bluepill_f103c8/lib23b/libboost_config.a
Indexing .pioenvs/bluepill_f103c8/libd23/libboost_preprocessor.a
Indexing .pioenvs/bluepill_f103c8/lib23b/libboost_config.a
Archiving .pioenvs/bluepill_f103c8/lib186/libboost_static_assert.a
Archiving .pioenvs/bluepill_f103c8/lib047/libboost_type_traits.a
Archiving .pioenvs/bluepill_f103c8/libf3d/libboost_assert.a
Indexing .pioenvs/bluepill_f103c8/lib186/libboost_static_assert.a
Archiving .pioenvs/bluepill_f103c8/lib3c5/libboost_core.a
Indexing .pioenvs/bluepill_f103c8/lib047/libboost_type_traits.a
Archiving .pioenvs/bluepill_f103c8/libd30/libboost_utility.a
Indexing .pioenvs/bluepill_f103c8/libf3d/libboost_assert.a
Indexing .pioenvs/bluepill_f103c8/lib3c5/libboost_core.a
Indexing .pioenvs/bluepill_f103c8/libd30/libboost_utility.a
Archiving .pioenvs/bluepill_f103c8/lib64e/libboost_predef.a
Archiving .pioenvs/bluepill_f103c8/libe2b/libboost_mpl.a
Archiving .pioenvs/bluepill_f103c8/lib95e/libboost_detail.a
Archiving .pioenvs/bluepill_f103c8/lib609/libboost_iterator.a
Indexing .pioenvs/bluepill_f103c8/lib64e/libboost_predef.a
Archiving .pioenvs/bluepill_f103c8/libcea/libboost_parameter.a
Indexing .pioenvs/bluepill_f103c8/libe2b/libboost_mpl.a
Indexing .pioenvs/bluepill_f103c8/lib95e/libboost_detail.a
Indexing .pioenvs/bluepill_f103c8/lib609/libboost_iterator.a
Indexing .pioenvs/bluepill_f103c8/libcea/libboost_parameter.a
Archiving .pioenvs/bluepill_f103c8/lib7b0/libboost_lockfree.a
Compiling .pioenvs/bluepill_f103c8/libd9b/logger/logger.o
Indexing .pioenvs/bluepill_f103c8/lib7b0/libboost_lockfree.a
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmI2C.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmPWM.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmPin.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmSPI.o
Archiving .pioenvs/bluepill_f103c8/libd9b/liblogger.a
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmSingleWireSerial.o
Indexing .pioenvs/bluepill_f103c8/libd9b/liblogger.a
stm32/cm/CmSPI.cpp:85:20: warning: 'int codal::_cm::enable_clock(uint32_t)' defined but not used [-Wunused-function]
static int enable_clock(uint32_t instance)
^~~~~~~~~~~~
stm32/cm/CmSPI.cpp:69:21: warning: 'codal::_cm::instances' defined but not used [-Wunused-variable]
static SPI *instances[4];
^~~~~~~~~
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmTimer.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmUSB.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/DynamicPwm.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/codal_target_hal.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/dma.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/irq.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pinmap.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pinmap_common.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pwmout_api.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pwmout_device.o
stm32/cm/irq.c: In function 'SysTick_Handler':
stm32/cm/irq.c:15:5: warning: implicit declaration of function 'HAL_IncTick' [-Wimplicit-function-declaration]
HAL_IncTick();
^~~~~~~~~~~
In file included from stm32/cm/pinmap.c:32:0:
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalCompat.o
stm32/cm/pin_device.h: In function 'stm_pin_SetAFPin':
stm32/cm/pin_device.h:68:9: warning: implicit declaration of function 'LL_GPIO_SetAFPin_8_15' [-Wimplicit-function-declaration]
LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
^~~~~~~~~~~~~~~~~~~~~
Archiving .pioenvs/bluepill_f103c8/lib739/libcm.a
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalComponent.o
stm32/cm/pin_device.h:70:9: warning: implicit declaration of function 'LL_GPIO_SetAFPin_0_7' [-Wimplicit-function-declaration]
LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
Indexing .pioenvs/bluepill_f103c8/lib739/libcm.a
^~~~~~~~~~~~~~~~~~~~
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalDevice.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalDmesg.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalFiber.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalHeapAllocator.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalListener.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalUtil.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/MemberFunctionCallback.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/AbstractButton.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Accelerometer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/CodalUSB.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Compass.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Display.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Gyroscope.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/I2C.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/SPI.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Sensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Timer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/AnalogSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/AnimatedDisplay.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/AsciiKeyMap.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/Button.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/FXOS8700.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/FXOS8700Accelerometer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/FXOS8700Magnetometer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/GhostFAT.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HID.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HIDJoystick.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HIDKeyboard.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HIDMouse.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/LEDMatrix.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/LIS3DH.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/LinearAnalogSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/MessageBus.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/MultiButton.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/NonLinearAnalogSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/ST7735.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/StandardSPIFlash.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/TouchButton.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/TouchSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/USBMSC.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/DataStream.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/LevelDetector.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/LevelDetectorSPL.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/StreamNormalizer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/Synthesizer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/BitmapFont.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/CoordinateSystem.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/Event.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/Image.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/ManagedBuffer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/ManagedString.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/Matrix4.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/RefCounted.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/RefCountedInit.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/STM32BluePill.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/STM32BluePillIO.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/STM32BluePillPin.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/bluepill.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/led.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/timer.o
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_assert.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/lib3e7/libcodal-core.a
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_cbk.o
Indexing .pioenvs/bluepill_f103c8/lib3e7/libcodal-core.a
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_event.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_kernel.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_msgqueue.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_sem.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'queue_push':
lib/cocoOS_5.0.3/src/os_msgqueue.c:160:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
Archiving .pioenvs/bluepill_f103c8/lib271/libbluepill.a
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:160:17: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
Indexing .pioenvs/bluepill_f103c8/lib271/libbluepill.a
uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_task.o
^
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msg_receive':
lib/cocoOS_5.0.3/src/os_msgqueue.c:199:27: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:199:15: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:221:31: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
Compiling .pioenvs/bluepill_f103c8/lib91e/spiint/spiint.o
dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:221:19: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msgQ_tick':
lib/cocoOS_5.0.3/src/os_msgqueue.c:255:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:255:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'MsgQAllDelayed':
lib/cocoOS_5.0.3/src/os_msgqueue.c:284:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
Compiling .pioenvs/bluepill_f103c8/libc3f/simulator/simulator.o
pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:284:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
^
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib33e/uartint/uartint.o
Compiling .pioenvs/bluepill_f103c8/libea2/i2cint/i2cint.o
Archiving .pioenvs/bluepill_f103c8/lib91e/libspiint.a
Archiving .pioenvs/bluepill_f103c8/libc3f/libsimulator.a
Archiving .pioenvs/bluepill_f103c8/libea7/libcocoOS_5.0.3.a
Indexing .pioenvs/bluepill_f103c8/lib91e/libspiint.a
Indexing .pioenvs/bluepill_f103c8/libc3f/libsimulator.a
Indexing .pioenvs/bluepill_f103c8/libea7/libcocoOS_5.0.3.a
Compiling .pioenvs/bluepill_f103c8/lib9ba/porting/porting.o
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/BME280.o
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/BME280I2C.o
Archiving .pioenvs/bluepill_f103c8/libea2/libi2cint.a
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/BME280Spi.o
Indexing .pioenvs/bluepill_f103c8/libea2/libi2cint.a
lib/BME280/src/BME280.cpp: In member function 'bool BME280::WriteSettings()':
lib/BME280/src/BME280.cpp:98:1: warning: no return statement in function returning non-void [-Wreturn-type]
}
^
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/EnvironmentCalculations.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/adc.o
Archiving .pioenvs/bluepill_f103c8/lib9ba/libporting.a
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/adc_common_v1.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Indexing .pioenvs/bluepill_f103c8/lib9ba/libporting.a
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/can.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/libdc9/libBME280.a
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/desig.o
Indexing .pioenvs/bluepill_f103c8/libdc9/libBME280.a
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/ethernet.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/flash.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/gpio.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/rcc.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/rtc.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/timer.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/mac.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/mac_stm32fxx7.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/phy.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/phy_ksz8051mll.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/crc_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/dac_common_all.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/dma_common_l1f013.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/gpio_common_all.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/i2c_common_all.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/iwdg_common_all.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/pwr_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/spi_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/spi_common_l1f124.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/timer_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/usart_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/usart_common_f124.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/rcc_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/exti_common_all.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-staticCompiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/flash_common_f01.o
s' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_control.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_standard.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_msc.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/lib33e/libuartint.a
Indexing .pioenvs/bluepill_f103c8/lib33e/libuartint.a
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_fx07_common.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_f107.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/st_usbfs_core.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/st_usbfs_v1.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/vector.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/systick.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/scb.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/nvic.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/assert.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/sync.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/dwt.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/libFrameworkLibOpenCM3.a
Indexing .pioenvs/bluepill_f103c8/libFrameworkLibOpenCM3.a
Linking .pioenvs/bluepill_f103c8/firmware.elf
Memory region         Used Size  Region Size  %age Used
rom:       30612 B        64 KB     46.71%
ram:        4148 B        20 KB     20.25%
Checking size .pioenvs/bluepill_f103c8/firmware.elf
Building .pioenvs/bluepill_f103c8/firmware.bin
Memory Usage -> http://bit.ly/pio-memory-usage
DATA:    [==        ]  20.3% (used 4148 bytes from 20480 bytes)
PROGRAM: [=====     ]  46.7% (used 30580 bytes from 65536 bytes)
============================================================================================================================ [SUCCESS] Took 40.58 seconds ============================================================================================================================

Terminal will be reused by tasks, press any key to close it.
```