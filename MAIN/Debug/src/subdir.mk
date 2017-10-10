################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MAIN.cpp 

OBJS += \
./src/MAIN.o 

CPP_DEPS += \
./src/MAIN.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking ARM-Linux C++ Compiler'
	arm-softfloat-linux-gnueabi-g++ -c -I"C:/Users/Kate/Documents/GitHub/RangerBot/MAIN/include" -I"C:/Program Files (x86)/Armus/ArmusIDE/librairie/include" -ggdb --sysroot="C:/Program Files (x86)/Armus/ArmusIDE/toolchain/sysroot" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


