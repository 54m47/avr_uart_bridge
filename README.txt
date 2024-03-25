Project: UART Bridge with FreeRTOS on Arduino Mega

This project implements a UART bridge on an Arduino Mega 2560 using FreeRTOS, which allows for bidirectional communication between two UARTs. Data received on one UART is transmitted through the other and vice versa, showcasing the real-time capabilities of an RTOS.

Setup Instructions:
-------------------
1. Hardware Requirements:
   - Arduino Mega 2560
   - USB cable for programming and powering the Arduino
   - Optional: Wires for loopback test or connection cables for an external UART device
   - Arduino Uno for sending and receiving test messages (optional)

2. Software Requirements:
   - Arduino IDE or an AVR toolchain with Make
   - FreeRTOS library (miniAVRfreeRTOS), which can be obtained from:
     https://github.com/feilipu/miniAVRfreeRTOS
   - Source files from this project (main.c, Makefile, and miniAVRfreeRTOS folder)

3. Project Compilation:
   - Ensure the Makefile is in the same directory as your source files.
   - Adjust the PORT in the Makefile if your Arduino is not connected to COM7.
   - Navigate to your project directory in the command line interface.
   - Run `make all` to compile the project.
   - Use `make flash` to upload the compiled program to the Arduino Mega.
   - To clean up the build files, you can run `make clean`.

4. Wiring for Testing:
   - For a loopback test, connect Pin 18 (TX1) to Pin 19 (RX1) on the Arduino Mega.
   - For communication with an external device, connect TX1 and RX1 to the other device's RX and TX pins, respectively. Establish a common ground connection between the devices.

5. Testing and Operation:
   - Open the Serial Monitor in the Arduino IDE connected to the Arduino Mega with the baud rate set to 9600.
   - Power the Arduino Uno. You should see the message "Hello from Second Arduino" printed in the Serial Monitor, indicating that the Uno is sending the message to the Mega, and the Mega is relaying it back to the Serial Monitor.
   - For further testing, you can send data from the Serial Monitor to the Mega, and it should be relayed out through the Uno if you set up the loopback on the Uno or connect a second Serial Monitor session to the Uno.

Project Compilation and Uploading:
----------------------------------
- To compile and upload the project to an Arduino Mega, you will need to use `avr-gcc` for compiling and `avrdude` for uploading the firmware.

1. AVR-GCC Compiler:
   - Ensure that you have `avr-gcc` installed on your system. This is part of the AVR toolchain used for compiling AVR projects.
   - Verify that the `Makefile` is configured with the correct MCU type and clock speed (F_CPU). For Arduino Mega, `MCU = atmega2560` and typically `F_CPU = 16000000UL`.

2. AVRDUDE for Uploading:
   - `avrdude` is used for uploading compiled binaries to AVR microcontrollers.
   - Ensure that the `avrdude.conf` path in the `Makefile` matches the actual location of the file on your system. For Windows users, this might be within the Arduino installation directory, e.g., `"D:\Programmes\Arduino\hardware\tools\avr\etc\avrdude.conf"`.
   - The `Makefile` should specify the correct port for the Arduino Mega. Replace `PORT = COM7` with the port number that corresponds to your Arduino's connection.
   - If you encounter issues with uploading, verify that you have the right programmer (such as `wiring` for Arduino Mega) and the correct baud rate for the bootloader. Adjust the `BAUD` and `PROGRAMMER` variables in the `Makefile` as needed.

3. Uploading to Arduino Uno (for testing):
   - If using an Arduino Uno to receive and send data for testing the UART bridge, make sure to compile and upload the `arduino_minimal_example.ino` sketch to the Uno using the Arduino IDE.
   - Configure the Serial Monitor to the correct baud rate (9600 by default) to communicate with the Uno.

Troubleshooting:
----------------
- If you encounter errors during compilation, ensure that all paths in the `Makefile` are correct and that all required source files are present in the specified directories.
- If `avr-gcc` or `avrdude` cannot be found, ensure that their paths are correctly set in your system's environment variables, or provide the full path to these tools in the `Makefile`.
- For issues with `avrdude`, such as failing to recognize the connected device or reporting errors during upload, double-check the port, baud rate, and programmer settings. Also, ensure that no other applications (like the Arduino IDE's Serial Monitor) are using the COM port when you attempt to upload.
- Ensure that the Arduino board is not in use by another application and that the correct driver is installed if you're facing connection issues.

After ensuring the proper setup of the AVR toolchain and the `Makefile` configurations, use the command `make all` to compile the project and `make flash` to upload it to the Arduino Mega.

Understanding USART vs. UART:
------------------------------
In the context of this project, we refer to the communication interfaces on the Arduino Mega 2560 as UARTs for their role in asynchronous serial communication, which is the primary focus of this UART bridge project. However, the technical term used in the microcontroller documentation and libraries is often USART, which stands for Universal Synchronous/Asynchronous Receiver Transmitter.

The key difference between UART and USART is that USARTs are more versatile, supporting both synchronous and asynchronous communication. This means a USART can function as a UART but also has the capability for synchronous operations, where clock signals are used in addition to the data signals, allowing for higher data rates and efficiency in certain scenarios.

In the Arduino Mega 2560, the USART hardware is utilized in its asynchronous mode, effectively operating as a UART. This choice is made to simplify the communication process for most applications, including this UART bridge, where the asynchronous serial communication (without an additional clock signal) is sufficient and widely used for data transmission between devices.

The terminology might vary across different platforms and documentation, but the underlying principles of serial communication remain consistent. By using the asynchronous capabilities of the USARTs on the Arduino Mega 2560, we can achieve the desired functionality for the UART bridge project without the need for synchronous communication features.

