# MABPA - Brake with a Flex
> **Engineering Capstone Project | University of California, Santa Cruz | 2025**
> Technical Roles: Embedded Lead & Mechanical Support
> Management Roles: Financial Lead & Team Facilitator
### MABPA (Muscle-Activated Brake Prosthetic Attachment) uses bicep biosensors to control a bike’s hand brake — with just a flex of your arm.

## The Problem
* **Limited Acessibility**: Over 85,000 Americans undergo an upper limb amputations annually (Amputee Coalition).
* **High barrier to Entry**: Current cycling solutions require permeanent brake modifications with specialized systems.
* **Safety risks**: Existing adaptations can reduce braking braking precision or complicate operation.
* **Innovation gap**: Bio-powered advancements haven't reached adaptive sports like cycling.

## The Solution
MABPA is a prototype "plug-and-play" bio-powered brake control for adaptive cycling designed to:
* **Integrate** seamlessly with existing prosthetics for mountain biking.
* **Install** on standard bikes with no major modifications.
*  **Withstand** rugged trails and high-impact riding.
*  **Provide** reliable, intuitive control via bicep activation.


![MABPA Diagram Full Bike ](https://github.com/user-attachments/assets/7aa08a00-928a-4d6c-974a-c01d32196962)
**System Integration**: High-level overview of the MABPA prototype installed on a standard mountain bike, highlighting the non-invasive mounting and component layout.

### Performance Benchmarks
| Metric | Goal | Result |
| :---   | :--- | :---   |
| Footprint (Handlebar) | < 1.5" | 1.2" |
| Installation | Allen Key (4-6)mm | 5mm Allen Key |
| Impact Resistance | 1.63 MPa | 2.01 MPa |
| Latency | < 200ms | 165-180ms |
| Activation Sucess | > 70% | 78% |

## Hardware Specifications
* **sEMG Sensor**: Myoware 2.0 sEMG Sensor - *Non-invasive muscle signal acquisition via a secure arm band.*
* **Microcontroller**: Teensy 4.1 - *Real-time signal processing, threshold logic, and system health checks in a compact footprint.*
* **Control Interface**: ILI9341 Touchscreen - *Provides real-time feedback on battery life, signal strength, and system health to ensure rider confidence.*
* **Actuator**: SB-2290SG High-Torque Servo - *Translates signals into the mechanical force required to pull a standard brake lever.*
* **Power**: Venom 4000 2S LiPo Battery - *High-discharge current for consistent braking and 2+ hours of active ride time.*
* **Signal Wires**: 22AWG Shielded UL2464 - *Protects sensitive bio-signals and PWM output from interference (high SNR).*
* **Housing**: 3D Printed PETG - *Rugged, impact-resistant enclosure designed to protect internals during high-impact trail riding.*
* **Bike Clamps**: Motorcycle Mounting Brackets - *Heavy-duty hardware chosen to prevent slip under the servo’s peak torque.*

## System Architecture
<img width="1085" height="933" alt="v3 0 Final MABPA System Block Diagram drawio" src="https://github.com/user-attachments/assets/694bf787-7345-457f-87ab-35bdd5f1fca7" />
Diagram illustrating the signal path from sEMG acquisition (user's bicep) to mechanical brake activation, including power regulation.

## Software & Systems Architecture
As the Embedded Lead, I was responsible for the full lifecycle of our control system, from hardware selection and component validation to the final hierarchical state machine (HSM).
* **Component Selection**: Developed **Criteria Matrices** to evaluate hardware and software based on performance benchmarks:
  * **Microcontroller**: Optimized for **clock speed, ADC resolution, PWM precision,** and **communication channels (SPI)**.
  * **UI Display**: Evaluated **viewing angle, sunlight visibility,** and **durability** in rough terrain.
  * **System Power**: Compared **power consumption** and **footprint** to balance battery life with a compact bike-frame form factor.
  * **Development Enviornment **: Selected VS Code with PlatformIO to leverage **advanced library management** and **low-level hardware access**.
*  **Modular Validation**: Developed a suite of **Peripheral Test Harnesses** to isolate and calibrate the sEMG sensor, high-torque servo, touchscreen and battery.
  * 

