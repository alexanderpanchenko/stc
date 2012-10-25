################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../library/BowVector.cpp \
../library/Classifier.cpp \
../library/ClassifierManager.cpp \
../library/FreqVocabulary.cpp \
../library/TextExpander.cpp \
../library/Vocabulary.cpp \
../library/classifier_c.cpp \
../library/predict.cpp \
../library/share_headers.cpp \
../library/textclassifier.cpp \
../library/train.cpp 

OBJS += \
./src/BowVector.o \
./src/Classifier.o \
./src/ClassifierManager.o \
./src/FreqVocabulary.o \
./src/TextExpander.o \
./src/Vocabulary.o \
./src/classifier_c.o \
./src/predict.o \
./src/share_headers.o \
./src/textclassifier.o \
./src/train.o 

CPP_DEPS += \
./src/BowVector.d \
./src/Classifier.d \
./src/ClassifierManager.d \
./src/FreqVocabulary.d \
./src/TextExpander.d \
./src/Vocabulary.d \
./src/classifier_c.d \
./src/predict.d \
./src/share_headers.d \
./src/textclassifier.d \
./src/train.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../library/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -m32 -I./../include -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


