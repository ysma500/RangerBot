################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CapteurDeCouleur.cpp \
../src/MAIN.cpp \
../src/infrarouge.cpp 

OBJS += \
./src/CapteurDeCouleur.o \
./src/MAIN.o \
./src/infrarouge.o 

CPP_DEPS += \
./src/CapteurDeCouleur.d \
./src/MAIN.d \
./src/infrarouge.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking ARM-Linux C++ Compiler'
	arm-softfloat-linux-gnueabi-g++ -c -I"C:/Users/Kate/Documents/GitHub/RangerBot/MAIN/include" -I"C:/Program Files (x86)/ArmusIDE/librairie/include" -I"C:/Program Files (x86)/ArmusIDE/toolchain/sysroot/usr/include" -I"C:/Program Files (x86)/ArmusIDE/toolchain/sysroot/usr/include/c++/4.2.4" -I"C:/Program Files (x86)/ArmusIDE/librairie/include" -ggdb --sysroot="C:/Program Files (x86)/ArmusIDE/toolchain/sysroot" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


