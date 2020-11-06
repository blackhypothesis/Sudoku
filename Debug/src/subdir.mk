################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Board.cpp \
../src/Field.cpp \
../src/Resources.cpp \
../src/Sudoku.cpp 

OBJS += \
./src/Board.o \
./src/Field.o \
./src/Resources.o \
./src/Sudoku.o 

CPP_DEPS += \
./src/Board.d \
./src/Field.d \
./src/Resources.d \
./src/Sudoku.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


