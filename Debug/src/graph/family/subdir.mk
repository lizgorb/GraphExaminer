################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/graph/family/complete_family.cpp \
../src/graph/family/wheel_family.cpp 

OBJS += \
./src/graph/family/complete_family.o \
./src/graph/family/wheel_family.o 

CPP_DEPS += \
./src/graph/family/complete_family.d \
./src/graph/family/wheel_family.d 


# Each subdirectory must supply rules for building sources it contributes
src/graph/family/%.o: ../src/graph/family/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


