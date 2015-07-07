################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Exceptions.c" \
"../Sources/IntcInterrupts.c" \
"../Sources/adc_init.c" \
"../Sources/adc_trigger_init.c" \
"../Sources/basetimer.c" \
"../Sources/camera.c" \
"../Sources/cansp_init.c" \
"../Sources/charlcd.c" \
"../Sources/demoled.c" \
"../Sources/demoswitch.c" \
"../Sources/dspi_init.c" \
"../Sources/dspi_var.c" \
"../Sources/edma_init.c" \
"../Sources/emios_init.c" \
"../Sources/flash_init.c" \
"../Sources/flexcan_init.c" \
"../Sources/freerunadc.c" \
"../Sources/freerunusonic.c" \
"../Sources/i2c_init.c" \
"../Sources/i2c_util.c" \
"../Sources/intc_init.c" \
"../Sources/ivor_branch_table.c" \
"../Sources/linflex_init.c" \
"../Sources/linflex_var.c" \
"../Sources/main.c" \
"../Sources/motorcontrol.c" \
"../Sources/mpu_init.c" \
"../Sources/msr_init.c" \
"../Sources/myuart.c" \
"../Sources/pcu_init.c" \
"../Sources/pit_init.c" \
"../Sources/rappid_utils.c" \
"../Sources/rchw_init.c" \
"../Sources/rgm_init.c" \
"../Sources/romcopy.c" \
"../Sources/rotaryencoder.c" \
"../Sources/rtc_init.c" \
"../Sources/serialdebug.c" \
"../Sources/siu_init.c" \
"../Sources/stm_init.c" \
"../Sources/swt_init.c" \
"../Sources/sys_init.c" \
"../Sources/sysclk_init.c" \
"../Sources/youruart.c" \

C_SRCS += \
../Sources/Exceptions.c \
../Sources/IntcInterrupts.c \
../Sources/adc_init.c \
../Sources/adc_trigger_init.c \
../Sources/basetimer.c \
../Sources/camera.c \
../Sources/cansp_init.c \
../Sources/charlcd.c \
../Sources/demoled.c \
../Sources/demoswitch.c \
../Sources/dspi_init.c \
../Sources/dspi_var.c \
../Sources/edma_init.c \
../Sources/emios_init.c \
../Sources/flash_init.c \
../Sources/flexcan_init.c \
../Sources/freerunadc.c \
../Sources/freerunusonic.c \
../Sources/i2c_init.c \
../Sources/i2c_util.c \
../Sources/intc_init.c \
../Sources/ivor_branch_table.c \
../Sources/linflex_init.c \
../Sources/linflex_var.c \
../Sources/main.c \
../Sources/motorcontrol.c \
../Sources/mpu_init.c \
../Sources/msr_init.c \
../Sources/myuart.c \
../Sources/pcu_init.c \
../Sources/pit_init.c \
../Sources/rappid_utils.c \
../Sources/rchw_init.c \
../Sources/rgm_init.c \
../Sources/romcopy.c \
../Sources/rotaryencoder.c \
../Sources/rtc_init.c \
../Sources/serialdebug.c \
../Sources/siu_init.c \
../Sources/stm_init.c \
../Sources/swt_init.c \
../Sources/sys_init.c \
../Sources/sysclk_init.c \
../Sources/youruart.c \

OBJS += \
./Sources/Exceptions_c.obj \
./Sources/IntcInterrupts_c.obj \
./Sources/adc_init_c.obj \
./Sources/adc_trigger_init_c.obj \
./Sources/basetimer_c.obj \
./Sources/camera_c.obj \
./Sources/cansp_init_c.obj \
./Sources/charlcd_c.obj \
./Sources/demoled_c.obj \
./Sources/demoswitch_c.obj \
./Sources/dspi_init_c.obj \
./Sources/dspi_var_c.obj \
./Sources/edma_init_c.obj \
./Sources/emios_init_c.obj \
./Sources/flash_init_c.obj \
./Sources/flexcan_init_c.obj \
./Sources/freerunadc_c.obj \
./Sources/freerunusonic_c.obj \
./Sources/i2c_init_c.obj \
./Sources/i2c_util_c.obj \
./Sources/intc_init_c.obj \
./Sources/ivor_branch_table_c.obj \
./Sources/linflex_init_c.obj \
./Sources/linflex_var_c.obj \
./Sources/main_c.obj \
./Sources/motorcontrol_c.obj \
./Sources/mpu_init_c.obj \
./Sources/msr_init_c.obj \
./Sources/myuart_c.obj \
./Sources/pcu_init_c.obj \
./Sources/pit_init_c.obj \
./Sources/rappid_utils_c.obj \
./Sources/rchw_init_c.obj \
./Sources/rgm_init_c.obj \
./Sources/romcopy_c.obj \
./Sources/rotaryencoder_c.obj \
./Sources/rtc_init_c.obj \
./Sources/serialdebug_c.obj \
./Sources/siu_init_c.obj \
./Sources/stm_init_c.obj \
./Sources/swt_init_c.obj \
./Sources/sys_init_c.obj \
./Sources/sysclk_init_c.obj \
./Sources/youruart_c.obj \

OBJS_QUOTED += \
"./Sources/Exceptions_c.obj" \
"./Sources/IntcInterrupts_c.obj" \
"./Sources/adc_init_c.obj" \
"./Sources/adc_trigger_init_c.obj" \
"./Sources/basetimer_c.obj" \
"./Sources/camera_c.obj" \
"./Sources/cansp_init_c.obj" \
"./Sources/charlcd_c.obj" \
"./Sources/demoled_c.obj" \
"./Sources/demoswitch_c.obj" \
"./Sources/dspi_init_c.obj" \
"./Sources/dspi_var_c.obj" \
"./Sources/edma_init_c.obj" \
"./Sources/emios_init_c.obj" \
"./Sources/flash_init_c.obj" \
"./Sources/flexcan_init_c.obj" \
"./Sources/freerunadc_c.obj" \
"./Sources/freerunusonic_c.obj" \
"./Sources/i2c_init_c.obj" \
"./Sources/i2c_util_c.obj" \
"./Sources/intc_init_c.obj" \
"./Sources/ivor_branch_table_c.obj" \
"./Sources/linflex_init_c.obj" \
"./Sources/linflex_var_c.obj" \
"./Sources/main_c.obj" \
"./Sources/motorcontrol_c.obj" \
"./Sources/mpu_init_c.obj" \
"./Sources/msr_init_c.obj" \
"./Sources/myuart_c.obj" \
"./Sources/pcu_init_c.obj" \
"./Sources/pit_init_c.obj" \
"./Sources/rappid_utils_c.obj" \
"./Sources/rchw_init_c.obj" \
"./Sources/rgm_init_c.obj" \
"./Sources/romcopy_c.obj" \
"./Sources/rotaryencoder_c.obj" \
"./Sources/rtc_init_c.obj" \
"./Sources/serialdebug_c.obj" \
"./Sources/siu_init_c.obj" \
"./Sources/stm_init_c.obj" \
"./Sources/swt_init_c.obj" \
"./Sources/sys_init_c.obj" \
"./Sources/sysclk_init_c.obj" \
"./Sources/youruart_c.obj" \

C_DEPS += \
./Sources/Exceptions_c.d \
./Sources/IntcInterrupts_c.d \
./Sources/adc_init_c.d \
./Sources/adc_trigger_init_c.d \
./Sources/basetimer_c.d \
./Sources/camera_c.d \
./Sources/cansp_init_c.d \
./Sources/charlcd_c.d \
./Sources/demoled_c.d \
./Sources/demoswitch_c.d \
./Sources/dspi_init_c.d \
./Sources/dspi_var_c.d \
./Sources/edma_init_c.d \
./Sources/emios_init_c.d \
./Sources/flash_init_c.d \
./Sources/flexcan_init_c.d \
./Sources/freerunadc_c.d \
./Sources/freerunusonic_c.d \
./Sources/i2c_init_c.d \
./Sources/i2c_util_c.d \
./Sources/intc_init_c.d \
./Sources/ivor_branch_table_c.d \
./Sources/linflex_init_c.d \
./Sources/linflex_var_c.d \
./Sources/main_c.d \
./Sources/motorcontrol_c.d \
./Sources/mpu_init_c.d \
./Sources/msr_init_c.d \
./Sources/myuart_c.d \
./Sources/pcu_init_c.d \
./Sources/pit_init_c.d \
./Sources/rappid_utils_c.d \
./Sources/rchw_init_c.d \
./Sources/rgm_init_c.d \
./Sources/romcopy_c.d \
./Sources/rotaryencoder_c.d \
./Sources/rtc_init_c.d \
./Sources/serialdebug_c.d \
./Sources/siu_init_c.d \
./Sources/stm_init_c.d \
./Sources/swt_init_c.d \
./Sources/sys_init_c.d \
./Sources/sysclk_init_c.d \
./Sources/youruart_c.d \

OBJS_OS_FORMAT += \
./Sources/Exceptions_c.obj \
./Sources/IntcInterrupts_c.obj \
./Sources/adc_init_c.obj \
./Sources/adc_trigger_init_c.obj \
./Sources/basetimer_c.obj \
./Sources/camera_c.obj \
./Sources/cansp_init_c.obj \
./Sources/charlcd_c.obj \
./Sources/demoled_c.obj \
./Sources/demoswitch_c.obj \
./Sources/dspi_init_c.obj \
./Sources/dspi_var_c.obj \
./Sources/edma_init_c.obj \
./Sources/emios_init_c.obj \
./Sources/flash_init_c.obj \
./Sources/flexcan_init_c.obj \
./Sources/freerunadc_c.obj \
./Sources/freerunusonic_c.obj \
./Sources/i2c_init_c.obj \
./Sources/i2c_util_c.obj \
./Sources/intc_init_c.obj \
./Sources/ivor_branch_table_c.obj \
./Sources/linflex_init_c.obj \
./Sources/linflex_var_c.obj \
./Sources/main_c.obj \
./Sources/motorcontrol_c.obj \
./Sources/mpu_init_c.obj \
./Sources/msr_init_c.obj \
./Sources/myuart_c.obj \
./Sources/pcu_init_c.obj \
./Sources/pit_init_c.obj \
./Sources/rappid_utils_c.obj \
./Sources/rchw_init_c.obj \
./Sources/rgm_init_c.obj \
./Sources/romcopy_c.obj \
./Sources/rotaryencoder_c.obj \
./Sources/rtc_init_c.obj \
./Sources/serialdebug_c.obj \
./Sources/siu_init_c.obj \
./Sources/stm_init_c.obj \
./Sources/swt_init_c.obj \
./Sources/sys_init_c.obj \
./Sources/sysclk_init_c.obj \
./Sources/youruart_c.obj \

C_DEPS_QUOTED += \
"./Sources/Exceptions_c.d" \
"./Sources/IntcInterrupts_c.d" \
"./Sources/adc_init_c.d" \
"./Sources/adc_trigger_init_c.d" \
"./Sources/basetimer_c.d" \
"./Sources/camera_c.d" \
"./Sources/cansp_init_c.d" \
"./Sources/charlcd_c.d" \
"./Sources/demoled_c.d" \
"./Sources/demoswitch_c.d" \
"./Sources/dspi_init_c.d" \
"./Sources/dspi_var_c.d" \
"./Sources/edma_init_c.d" \
"./Sources/emios_init_c.d" \
"./Sources/flash_init_c.d" \
"./Sources/flexcan_init_c.d" \
"./Sources/freerunadc_c.d" \
"./Sources/freerunusonic_c.d" \
"./Sources/i2c_init_c.d" \
"./Sources/i2c_util_c.d" \
"./Sources/intc_init_c.d" \
"./Sources/ivor_branch_table_c.d" \
"./Sources/linflex_init_c.d" \
"./Sources/linflex_var_c.d" \
"./Sources/main_c.d" \
"./Sources/motorcontrol_c.d" \
"./Sources/mpu_init_c.d" \
"./Sources/msr_init_c.d" \
"./Sources/myuart_c.d" \
"./Sources/pcu_init_c.d" \
"./Sources/pit_init_c.d" \
"./Sources/rappid_utils_c.d" \
"./Sources/rchw_init_c.d" \
"./Sources/rgm_init_c.d" \
"./Sources/romcopy_c.d" \
"./Sources/rotaryencoder_c.d" \
"./Sources/rtc_init_c.d" \
"./Sources/serialdebug_c.d" \
"./Sources/siu_init_c.d" \
"./Sources/stm_init_c.d" \
"./Sources/swt_init_c.d" \
"./Sources/sys_init_c.d" \
"./Sources/sysclk_init_c.d" \
"./Sources/youruart_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Exceptions_c.obj: ../Sources/Exceptions.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Exceptions.args" -o "Sources/Exceptions_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/IntcInterrupts_c.obj: ../Sources/IntcInterrupts.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/IntcInterrupts.args" -o "Sources/IntcInterrupts_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/adc_init_c.obj: ../Sources/adc_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/adc_init.args" -o "Sources/adc_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/adc_trigger_init_c.obj: ../Sources/adc_trigger_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/adc_trigger_init.args" -o "Sources/adc_trigger_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/basetimer_c.obj: ../Sources/basetimer.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/basetimer.args" -o "Sources/basetimer_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/camera_c.obj: ../Sources/camera.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/camera.args" -o "Sources/camera_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/cansp_init_c.obj: ../Sources/cansp_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/cansp_init.args" -o "Sources/cansp_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/charlcd_c.obj: ../Sources/charlcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/charlcd.args" -o "Sources/charlcd_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/demoled_c.obj: ../Sources/demoled.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/demoled.args" -o "Sources/demoled_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/demoswitch_c.obj: ../Sources/demoswitch.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/demoswitch.args" -o "Sources/demoswitch_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/dspi_init_c.obj: ../Sources/dspi_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/dspi_init.args" -o "Sources/dspi_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/dspi_var_c.obj: ../Sources/dspi_var.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/dspi_var.args" -o "Sources/dspi_var_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/edma_init_c.obj: ../Sources/edma_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/edma_init.args" -o "Sources/edma_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/emios_init_c.obj: ../Sources/emios_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/emios_init.args" -o "Sources/emios_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/flash_init_c.obj: ../Sources/flash_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/flash_init.args" -o "Sources/flash_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/flexcan_init_c.obj: ../Sources/flexcan_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/flexcan_init.args" -o "Sources/flexcan_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/freerunadc_c.obj: ../Sources/freerunadc.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/freerunadc.args" -o "Sources/freerunadc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/freerunusonic_c.obj: ../Sources/freerunusonic.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/freerunusonic.args" -o "Sources/freerunusonic_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/i2c_init_c.obj: ../Sources/i2c_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/i2c_init.args" -o "Sources/i2c_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/i2c_util_c.obj: ../Sources/i2c_util.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/i2c_util.args" -o "Sources/i2c_util_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/intc_init_c.obj: ../Sources/intc_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/intc_init.args" -o "Sources/intc_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/ivor_branch_table_c.obj: ../Sources/ivor_branch_table.c
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/ivor_branch_table.args" -o "Sources/ivor_branch_table_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/linflex_init_c.obj: ../Sources/linflex_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/linflex_init.args" -o "Sources/linflex_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/linflex_var_c.obj: ../Sources/linflex_var.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/linflex_var.args" -o "Sources/linflex_var_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/motorcontrol_c.obj: ../Sources/motorcontrol.c
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/motorcontrol.args" -o "Sources/motorcontrol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/mpu_init_c.obj: ../Sources/mpu_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #27 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/mpu_init.args" -o "Sources/mpu_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/msr_init_c.obj: ../Sources/msr_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #28 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/msr_init.args" -o "Sources/msr_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/myuart_c.obj: ../Sources/myuart.c
	@echo 'Building file: $<'
	@echo 'Executing target #29 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/myuart.args" -o "Sources/myuart_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/pcu_init_c.obj: ../Sources/pcu_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #30 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/pcu_init.args" -o "Sources/pcu_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/pit_init_c.obj: ../Sources/pit_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #31 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/pit_init.args" -o "Sources/pit_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/rappid_utils_c.obj: ../Sources/rappid_utils.c
	@echo 'Building file: $<'
	@echo 'Executing target #32 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/rappid_utils.args" -o "Sources/rappid_utils_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/rchw_init_c.obj: ../Sources/rchw_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #33 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/rchw_init.args" -o "Sources/rchw_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/rgm_init_c.obj: ../Sources/rgm_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #34 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/rgm_init.args" -o "Sources/rgm_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/romcopy_c.obj: ../Sources/romcopy.c
	@echo 'Building file: $<'
	@echo 'Executing target #35 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/romcopy.args" -o "Sources/romcopy_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/rotaryencoder_c.obj: ../Sources/rotaryencoder.c
	@echo 'Building file: $<'
	@echo 'Executing target #36 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/rotaryencoder.args" -o "Sources/rotaryencoder_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/rtc_init_c.obj: ../Sources/rtc_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #37 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/rtc_init.args" -o "Sources/rtc_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/serialdebug_c.obj: ../Sources/serialdebug.c
	@echo 'Building file: $<'
	@echo 'Executing target #38 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/serialdebug.args" -o "Sources/serialdebug_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/siu_init_c.obj: ../Sources/siu_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #39 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/siu_init.args" -o "Sources/siu_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/stm_init_c.obj: ../Sources/stm_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #40 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/stm_init.args" -o "Sources/stm_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/swt_init_c.obj: ../Sources/swt_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #41 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/swt_init.args" -o "Sources/swt_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/sys_init_c.obj: ../Sources/sys_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #42 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/sys_init.args" -o "Sources/sys_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/sysclk_init_c.obj: ../Sources/sysclk_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #43 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/sysclk_init.args" -o "Sources/sysclk_init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/youruart_c.obj: ../Sources/youruart.c
	@echo 'Building file: $<'
	@echo 'Executing target #44 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/youruart.args" -o "Sources/youruart_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


