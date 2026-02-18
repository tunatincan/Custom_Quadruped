# Open-Architecture Quadruped Platform for Dynamic Locomotion & Embodied AI
**(Custom Hardware for Torque-Level Control Research)**

## 1. Project Overview
**"Why build a custom robot?"**
Commercial quadrupeds often rely on "black-box" controllers, limiting research on low-level dynamics and novel learning algorithms.
This project aims to develop a **fully open-source, 12-DOF quadruped robot** designed to validate:
- **Dynamic Locomotion:** High-bandwidth torque control for robust walking on unstructured terrain.
- **Embodied AI:** Sim-to-Real transfer of learning-based policies (RL / Active Inference).

## 2. Hardware Design Philosophy
> **Goal:** Maximizing transparency and back-drivability for interaction.

- **Low Inertia Legs:** Co-axial actuation with fuselage-mounted motors to minimize leg mass.
- **Transmission:** **Cable-driven Capstan Mechanism** to eliminate backlash and ensure high back-drivability.
- **Proprioception:** Estimating ground reaction forces (GRF) purely via current sensing (FOC) without external torque sensors.
- **Open Architecture:** Full access to low-level control loops (1kHz) via CAN/UART.
<img width="2560" height="1082" alt="Image" src="https://github.com/user-attachments/assets/e8031cea-083f-4d49-baf5-3c66b0d53155" />

*(Caption: Full-body design for high-dynamic maneuvers)*

## 3. Current Development Status
> **Phase 1 Complete: Single Leg Module Verification**

Successfully fabricated and verified the core actuator module.

![Image](https://github.com/user-attachments/assets/ba6e91bb-07b7-4ef0-99e3-27a2363621ef)
*(Caption: Fabricated single leg module and torque test setup)*

## 4. Tech Stack
- **Hardware:** High-Torque BLDC Motors, ODrive v3.6 (FOC), Custom CNC/3D Printed Parts
- **Main Controller:** Nvidia AGX Xaiver / Rasberry Pi 5
- **Software:** ROS 2 (Humble)

Explanation for files  
custom_quadruped/  
├── cad/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; # Fusion 360 Original Files  
├── config/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;             # ODrive Setup code including parameters  
├── docs/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;               # Schematics, BOM (To be updated)  
├── hardware/ &nbsp;&nbsp;          # 3D printer setting values  
├── launch/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;            # Launch file for ROS 2 (To be updated)  
├── meshes/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;             # STL Files for Each parts (Seperated files will be uploaded later)  
├── scripts/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;            # Simple Motor test script to see if the motor is working  
├── urdf/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;               # Xacro/URDF - To be updated  
├── LICENSE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;            # MIT License file  
└── README.md           # Project Overview, current state, Explanation for files  
