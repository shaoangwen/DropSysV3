################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/board.c \
../drivers/drv_adc.c \
../drivers/drv_clk.c \
../drivers/drv_common.c \
../drivers/drv_eth.c \
../drivers/drv_flash_f4.c \
../drivers/drv_gpio.c \
../drivers/drv_hwtimer.c \
../drivers/drv_pwm.c \
../drivers/drv_qspi.c \
../drivers/drv_rtc.c \
../drivers/drv_sdio.c \
../drivers/drv_soft_i2c.c \
../drivers/drv_spi.c \
../drivers/drv_usart.c \
../drivers/drv_usbd.c \
../drivers/drv_usbh.c \
../drivers/drv_wdt.c 

OBJS += \
./drivers/board.o \
./drivers/drv_adc.o \
./drivers/drv_clk.o \
./drivers/drv_common.o \
./drivers/drv_eth.o \
./drivers/drv_flash_f4.o \
./drivers/drv_gpio.o \
./drivers/drv_hwtimer.o \
./drivers/drv_pwm.o \
./drivers/drv_qspi.o \
./drivers/drv_rtc.o \
./drivers/drv_sdio.o \
./drivers/drv_soft_i2c.o \
./drivers/drv_spi.o \
./drivers/drv_usart.o \
./drivers/drv_usbd.o \
./drivers/drv_usbh.o \
./drivers/drv_wdt.o 

C_DEPS += \
./drivers/board.d \
./drivers/drv_adc.d \
./drivers/drv_clk.d \
./drivers/drv_common.d \
./drivers/drv_eth.d \
./drivers/drv_flash_f4.d \
./drivers/drv_gpio.d \
./drivers/drv_hwtimer.d \
./drivers/drv_pwm.d \
./drivers/drv_qspi.d \
./drivers/drv_rtc.d \
./drivers/drv_sdio.d \
./drivers/drv_soft_i2c.d \
./drivers/drv_spi.d \
./drivers/drv_usart.d \
./drivers/drv_usbd.d \
./drivers/drv_usbh.d \
./drivers/drv_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F4 -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include\config" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\RTOS\Template" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc\Legacy" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\applications" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\spi" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\finsh" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\libc\compilers\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\cortex-m4" -include"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

