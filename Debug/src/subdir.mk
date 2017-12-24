################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Colorer.cpp \
../src/CompleteClass.cpp \
../src/EvenHoleFree.cpp \
../src/GraphClass.cpp \
../src/GraphGenerator.cpp \
../src/HoleDetector.cpp \
../src/InducedCheck.cpp 

OBJS += \
./src/Colorer.o \
./src/CompleteClass.o \
./src/EvenHoleFree.o \
./src/GraphClass.o \
./src/GraphGenerator.o \
./src/HoleDetector.o \
./src/InducedCheck.o 

CPP_DEPS += \
./src/Colorer.d \
./src/CompleteClass.d \
./src/EvenHoleFree.d \
./src/GraphClass.d \
./src/GraphGenerator.d \
./src/HoleDetector.d \
./src/InducedCheck.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"D:\Dev\cygwin64\usr\local\include\boost" -I"D:\Dev\cygwin64\usr\local\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


