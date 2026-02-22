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
*(Caption: Fabricated single leg module)*

## 4. Tech Stack
- **Hardware:** High-Torque BLDC Motors, ODrive v3.6 (FOC), Custom CNC/3D Printed Parts
- **Main Controller:** Nvidia AGX Xaiver / Rasberry Pi 5
- **Software:** ROS 2 (Humble)
- **Removed:** As MKS Odrive is More than enough for motor controll, removed STM32 layer for direct PC-to-ODrive CAN communication, reducing latency and hardware complexity


### Hardware Iteration & Manufacturing Log (v1.2) - Feb 22~23, 2026
1. Design for Assembly (DfA) & Modularization
- Issue: During the physical assembly, I realized that fastening the 70mm M4 screws to connect the limbs required completely disassembling the robot leg and reassembling it directly on the chassis—a severe maintenance bottleneck.
- Solution: Completely modularized the leg/arm bases. The interface parts are now mounted to the chassis first, allowing the fully pre-assembled limbs to be attached instantly. Furthermore, the outer housing screw directions were inverted (outside-in) for easier access.

2. Thermal Management & Wiring Routing
- Braking Resistor Relocation: Recognized the severe heat dissipation of the braking resistors. To prevent thermal damage and interference with the DC-DC step-down converter, the resistors were relocated to the opposite side of the motor drivers, optimizing the thermal layout.

3. Sensor Protection & Vibration Isolation
- LiDAR Roof Guard: Recalling a previous rollover incident with the Unitree Go2, I designed a custom roof-style LiDAR protector. It is strategically supported from the rear to avoid obstructing the LiDAR's effective FoV.

- Sensor Mounts: Added dedicated mounting points for the RTK-GPS and WT901B IMU, including TPU-printed damping feet to isolate the IMU from mechanical vibrations.

- TPU Feet: Designed custom TPU bumpers on the chasis to absorb shock and prevent the robot from breaking on sudden 'giveaways

4. Clearance Tuning & Rapid Workaround
- Interference Resolution: Identified multiple interference points during assembly (XT90 housing, big drum/motor joint, and a 3mm tolerance error in the limb-chassis link).

- Action Taken: To save rapid-prototyping resources (time and filament), I performed on-the-fly physical modifications (e.g., trimming the small drum) for the current prototype, while permanently fixing all 1mm/3mm clearances and stopper constraints in the final CAD model.

- Tooling Troubleshooting: 3D Printer Thermal UpgradeIssue: While printing new PETG parts at an 80°C bed temperature on my Sindoh WOX 2X, the previously installed PLA holders for the Y-axis LM8UU linear bearings deformed due to exceeding PLA's Glass Transition Temperature ($T_g$).

- Solution: Paused the project, printed high-temp PETG replacement holders on a secondary printer, and installed them with fiber washers for clearance. I also wrapped the assembly in Kapton tape for superior thermal insulation, fully upgrading the printer for continuous high-temperature manufacturing.

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
