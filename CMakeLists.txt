cmake_minimum_required(VERSION 4.2.0)
project(ElectricMotorControlAndSimulation)

add_executable(EMCS src/main.cpp)

target_sources(EMCS PRIVATE
  src/motor-simulator/motorSimulator.cpp
)

target_include_directories(EMCS PRIVATE src)
