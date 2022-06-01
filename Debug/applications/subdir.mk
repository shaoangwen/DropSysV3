################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applications/Door_control.c \
../applications/I2C2.c \
../applications/app_level.c \
../applications/cmd_handler.c \
../applications/configration.c \
../applications/io_define.c \
../applications/main.c \
../applications/motor_control.c \
../applications/motor_test.c \
../applications/pressure_control.c \
../applications/pump_control.c \
../applications/pwm.c \
../applications/serial_interface.c \
../applications/tmc5130_spi.c \
../applications/valves_control.c 

OBJS += \
./applications/Door_control.o \
./applications/I2C2.o \
./applications/app_level.o \
./applications/cmd_handler.o \
./applications/configration.o \
./applications/io_define.o \
./applications/main.o \
./applications/motor_control.o \
./applications/motor_test.o \
./applications/pressure_control.o \
./applications/pump_control.o \
./applications/pwm.o \
./applications/serial_interface.o \
./applications/tmc5130_spi.o \
./applications/valves_control.o 

C_DEPS += \
./applications/Door_control.d \
./applications/I2C2.d \
./applications/app_level.d \
./applications/cmd_handler.d \
./applications/configration.d \
./applications/io_define.d \
./applications/main.d \
./applications/motor_control.d \
./applications/motor_test.d \
./applications/pressure_control.d \
./applications/pump_control.d \
./applications/pwm.d \
./applications/serial_interface.d \
./applications/tmc5130_spi.d \
./applications/valves_control.d 


# Each subdirectory must supply rules for building sources it contributes
applications/%.o: ../applications/%.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F4 -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include\config" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\RTOS\Template" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc\Legacy" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\applications" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\spi" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\finsh" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\libc\compilers\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\cortex-m4" -include"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
applications/motor_control.o: ../applications/motor_control.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F4 -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include\config" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\RTOS\Template" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc\Legacy" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\applications" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\spi" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\finsh" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\libc\compilers\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\cortex-m4" -include"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rtconfig_preinc.h" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"applications/motor_control.d" -c -o "$@" "$<"

