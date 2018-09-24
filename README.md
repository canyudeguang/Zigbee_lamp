ZigbeeLamp
=====================================================================
  This project is based on Qualcomm QCA4020 development kit whose name is Zigbee Lamp. The main objective of this demo is done
  by using an application on mobile phone to connect to the QCA4020 via BLE(Bluetooth Low Energy),and there will PWM wave output
  through GPIO of QCA4020,at the same time,servo motor will have angle change at different PWM situations.Actually the servo motor
  of this demo is designed to simulate a smart lock.  


Instruction:
----------------------

Hardware requirements
--------------
QCA4020 development Kit, Servo motor, Adaptor, iPhone

Software requirements
--------  
1)toolchain
--
The toochain can be downloaded from the ARM website at:  
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads   
Add the path to toolchain binaries to 'PATH' environment variable.  
	  
2)Python
--
Some of the support scripts are Python-based, Python 2.7.2, or higher. You can download the Python official website.  
After installation, add the path to python to 'PATH' environment variable.  

Assembly Instructions
-----------------------------------------------------------------------------
Install the jumper of QCA4020 development kit and connect the servo motor to QCA4020 as described in the section  
"Build/Assembly Instructions" of "CDB20_Project_Content_SmartLock.docx".

Usage Instructions
--------------------------
(1)Download the code from github according to this repositary.  

(2)Compile the code and flash the image as described in "Hello world with QCA4020 Development" project. 

(3)Power on the QCA4020  

(4)Open the app named "LightBlue" on iPhone, and "LightBlue" will search BLE automatically, then connect the spot named "lpw-spple-demo".  

(5)The service we used is called "UUID:1815", and we use the fourth characteristic to control motor.  

(6)We input "FFFF", the lock will be "open", and "0000" the lock will be "closed". 
