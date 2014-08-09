################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/wavegen.cpp \
../src/wgmodulator.cpp \
../src/wgwave.cpp 

OBJS += \
./src/wavegen.o \
./src/wgmodulator.o \
./src/wgwave.o 

CPP_DEPS += \
./src/wavegen.d \
./src/wgmodulator.d \
./src/wgwave.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -includeportaudio.h -includepa_linux_alsa.h -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


