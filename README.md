Arduino CAN Bus Controller with SD Card Integration
==============

This project demonstrates an Arduino-based system that uses a CAN shield to communicate over a CAN bus. 
The system features four input signals (2 switches and 2 pots) for simulating different vehicle functionalities, and generate predefined CAN messages. Additionally, it includes the ability to read an SD card, parse its content (.trc), and convert it into CAN messages.

# Features
CAN Bus Communication: Sends and receives messages over the CAN bus using an Arduino CAN shield.
Switch Inputs: Includes four switches for sending predefined CAN signals.
SD Card Integration: Reads data from an SD card, processes its content, and transmits it as CAN messages.

# Hardware Requirements
Arduino Board (e.g., Arduino Uno)
CAN Shield (e.g., MCP2515-based or similar) with SD Card slot
SD Card with preloaded data (.trc)
Buttons and Pots
Power Supply
