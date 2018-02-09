################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/graph_examiner.cpp \
../src/graph_generator.cpp \
../src/induced_check.cpp 

OBJS += \
./src/graph_examiner.o \
./src/graph_generator.o \
./src/induced_check.o 

CPP_DEPS += \
./src/graph_examiner.d \
./src/graph_generator.d \
./src/induced_check.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"D:\Dev\cygwin64\usr\local\include\boost" -I"D:\Dev\cygwin64\usr\local\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


