################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/drivers/src/completion.c \
../rt-thread/components/drivers/src/dataqueue.c \
../rt-thread/components/drivers/src/pipe.c \
../rt-thread/components/drivers/src/ringblk_buf.c \
../rt-thread/components/drivers/src/ringbuffer.c \
../rt-thread/components/drivers/src/waitqueue.c \
../rt-thread/components/drivers/src/workqueue.c 

OBJS += \
./rt-thread/components/drivers/src/completion.o \
./rt-thread/components/drivers/src/dataqueue.o \
./rt-thread/components/drivers/src/pipe.o \
./rt-thread/components/drivers/src/ringblk_buf.o \
./rt-thread/components/drivers/src/ringbuffer.o \
./rt-thread/components/drivers/src/waitqueue.o \
./rt-thread/components/drivers/src/workqueue.o 

C_DEPS += \
./rt-thread/components/drivers/src/completion.d \
./rt-thread/components/drivers/src/dataqueue.d \
./rt-thread/components/drivers/src/pipe.d \
./rt-thread/components/drivers/src/ringblk_buf.d \
./rt-thread/components/drivers/src/ringbuffer.d \
./rt-thread/components/drivers/src/waitqueue.d \
./rt-thread/components/drivers/src/workqueue.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/drivers/src/%.o: ../rt-thread/components/drivers/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F4 -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\drivers\include\config" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\Include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\CMSIS\RTOS\Template" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\libraries\STM32F4xx_HAL_Driver\Inc\Legacy" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\applications" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx\Inc" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\cubemx" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\drivers\spi" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\finsh" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\components\libc\compilers\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\include" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\common" -I"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rt-thread\libcpu\arm\cortex-m4" -include"C:\home_of_wen\Documents\gobioX\Instruments\firmware\dsv3\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

