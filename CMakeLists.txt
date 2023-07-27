# -----------------------------------------------------------------------------
# Title  : Cmake makefile for stm32 compilation with cubeMX
# Author : Simone Ruffini <simone.ruffini.work@gmail.com>
# Date   : Wed May 17 10:37:43 PM CEST 2023
# Notes  :
#
# License:
#  "THE BEER-WARE LICENSE" (Revision 69):
#  Squadra Corse firmware team wrote this project. As long as you retain this
#  notice you can do whatever you want with this stuff. If we meet some day,
#  and you think this stuff is worth it, you can buy us a beer in return.
# Copyright squadracorsepolito.it
# -----------------------------------------------------------------------------

# Necessary tag
cmake_minimum_required(VERSION 3.16)

# Path where CMSIS/HAL will be searched
cmake_path(SET STM32_CUBE_F4_PATH "./")

# STM32-CMAKE internal variable: path to toolchain file for this project (if path changes update this variable)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stm32-cmake/cmake/stm32_gcc.cmake)

# Check that necessary parameters are given
string(STRIP "${PROJECT_NAME}" PROJECT_NAME)
if("${PROJECT_NAME}" STREQUAL "" ) 
    message(FATAL_ERROR "A non empty PROJECT_NAME variable must be given")
endif()

# Set project name
project(${PROJECT_NAME}) # given as an input parameter
enable_language(C ASM)

# Find CMSIS targets 
# Get CMSIS files for all STM32F4 family devices
find_package(CMSIS COMPONENTS STM32F4 REQUIRED) 

# Find HAL targets 
# Get HAL files for all STM32F4 family devices
find_package(HAL COMPONENTS STM32F4 REQUIRED)

# Set header directories 
set(PROJECT_INCLUDE_DIRECTORIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/SCan/SC22EVO/artifacts/c_source/
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/ 
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/timer_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/timebase
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/critical_section
)

# Get sources via globbing recursive search
file(GLOB_RECURSE PROJECT_C_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/SCan/SC22EVO/artifacts/c_source/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/timebase/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/timer_utils/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/stmlibs/critical_section/*.c
)

# Get asm sources via globbing search
file(GLOB PROJECT_ASM_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/*.s
)


# Commonn compile options must be before add_executable
# https://stackoverflow.com/questions/40516794/cmake-not-applying-compile-option-using-add-compile-options
 
# Optimizations
add_compile_options(-Og)
# C standard
add_compile_options(-std=gnu17)

# Debug compile options

# Produce debugging information in DWARF (version2) format
add_compile_options($<$<CONFIG:DEBUG>:-g> $<$<CONFIG:DEBUG>:-gdwarf-2>)

# Add executable target with correct name
set(EXECUTABLE_TARGET "${PROJECT_NAME}")
add_executable(${PROJECT_NAME})

set(EXECUTABLE_TARGET_SHIFTED "${PROJECT_NAME}_shifted")
add_executable(${EXECUTABLE_TARGET_SHIFTED})


target_compile_definitions(${EXECUTABLE_TARGET} PRIVATE
    STM32F446xx # must add this option because the target CMSIS::STM32::F446xx is not used since it brings with it the startup script
    USE_HAL_DRIVER)
target_compile_definitions(${EXECUTABLE_TARGET_SHIFTED} PRIVATE
    $<TARGET_PROPERTY:${EXECUTABLE_TARGET},COMPILE_DEFINITIONS>
)

# Add C and ASM target sources 
target_sources(${EXECUTABLE_TARGET} PRIVATE
    ${PROJECT_ASM_SOURCES}
    ${PROJECT_C_SOURCES})
target_sources(${EXECUTABLE_TARGET_SHIFTED} PRIVATE
    $<TARGET_PROPERTY:${EXECUTABLE_TARGET},SOURCES>
)

# Add include directories for the target compilation
target_include_directories(${EXECUTABLE_TARGET} PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES}
)
target_include_directories(${EXECUTABLE_TARGET_SHIFTED} PRIVATE
    $<TARGET_PROPERTY:${EXECUTABLE_TARGET},INCLUDE_DIRECTORIES>
)

# Add custom linker script (works only if you don't add target CMSIS::STM32::<full_device>)
stm32_add_linker_script(${EXECUTABLE_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32F446RETx_FLASH.ld)
stm32_add_linker_script(${EXECUTABLE_TARGET_SHIFTED} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32F446RETx_FLASH_shifted.ld)

target_link_libraries(${EXECUTABLE_TARGET}
    #CMSIS::STM32::F446RE
    #CMSIS::STM32::F446xx # this target adds common includes, compiler flags and startup file (TODO: change to STM32::F4, startup file should be added manually)
    CMSIS::STM32::F4
    HAL::STM32::F4::ADC
    HAL::STM32::F4::ADCEx
    HAL::STM32::F4::CAN
    HAL::STM32::F4::CORTEX
    HAL::STM32::F4::PWR
    HAL::STM32::F4::PWREx
    HAL::STM32::F4::DMA
    HAL::STM32::F4::DMAEx
    HAL::STM32::F4::EXTI
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::RCC
    HAL::STM32::F4::RCCEx
    HAL::STM32::F4::TIM
    HAL::STM32::F4::TIMEx
    HAL::STM32::F4::UART
    HAL::STM32::F4::SPI
    HAL::STM32::F4::IWDG
    STM32::Nano
    STM32::Nano::FloatPrint # Add printf/sprintf/... floating point functionality
    STM32::NoSys # already adds the -lnosys option
    #-lc # adds the -lc linking library
    #-lm # adds the -lm linging library
    #-lnosys # automatycally added by linking to NoSys
)
target_link_libraries(${EXECUTABLE_TARGET_SHIFTED} 
    $<TARGET_PROPERTY:${EXECUTABLE_TARGET},LINK_LIBRARIES>
)

stm32_generate_binary_file(${EXECUTABLE_TARGET})
stm32_generate_hex_file(${EXECUTABLE_TARGET})

stm32_print_size_of_target(${EXECUTABLE_TARGET})

stm32_generate_binary_file(${EXECUTABLE_TARGET_SHIFTED})
stm32_generate_hex_file(${EXECUTABLE_TARGET_SHIFTED})

stm32_print_size_of_target(${EXECUTABLE_TARGET_SHIFTED})

