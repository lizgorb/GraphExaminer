################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/graph/common_graphs.cpp \
../src/graph/subgraph.cpp \
../src/graph/subgraph_factory.cpp 

OBJS += \
./src/graph/common_graphs.o \
./src/graph/subgraph.o \
./src/graph/subgraph_factory.o 

CPP_DEPS += \
./src/graph/common_graphs.d \
./src/graph/subgraph.d \
./src/graph/subgraph_factory.d 


# Each subdirectory must supply rules for building sources it contributes
src/graph/%.o: ../src/graph/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


