# MABPA - Brake with a Flex
> **Engineering Capstone Project | University of California, Santa Cruz | Sept 2024 - June 2025**
### MABPA (Muscle-Activated Brake Prosthetic Attachment) uses bicep biosensors to control a bike’s hand brake — with just a flex of your arm.
![MABPA Poster Overview v2 1 3](https://github.com/user-attachments/assets/ae0df19f-624d-4b97-afd2-eb29841f43c2)
For a detailed technical breakdown of the MABPA project — from initial problem identification to final system verification — please use the dropdowns provided below:

<details>
<summary><b>View Project Context: The Problem & Solution</b></summary>  
 
## The Problem
* **Limited Accessibility**: Over 85,000 Americans undergo an upper limb amputations annually (Amputee Coalition).
* **High barrier to Entry**: Current cycling solutions require permanent brake modifications with specialized systems.
* **Safety risks**: Existing adaptations can reduce braking precision or complicate operation.
* **Innovation gap**: Bio-powered advancements haven't reached adaptive sports like cycling.

## The Solution
MABPA is a prototype "plug-and-play" bio-powered brake control for adaptive cycling designed to:
* **Integrate** seamlessly with existing prosthetics for mountain biking.
* **Install** on standard mountain bikes with no permanent modifications.
*  **Withstand** rugged trails and high-impact riding.
*  **Provide** reliable, intuitive control via bicep activation.
  </details>

<details>
<summary><b>View Team members & Contributors</b></summary> 

## Meet The Team
**Ashton Coons** (Embedded Systems Lead, Financial Lead, Team Facilitator)
**Benjamin Yang** ()
**Neo Ajogo** () 

## Mentorship & Special Thanks
**Stephen Petersen** Faculty Advisor and Honorary "Devil's Advocate"
**TA/s special thanks**
   </details>

<details>
<summary><b>View System Verification & Hardware Specs</b></summary>    
 
### System Verification Matrix
| Metric | Goal | Result |
| :---   | :--- | :---   |
| Footprint (Handlebar) | < 1.5" | 1.2" |
| Installation | Allen Key (4-6)mm | Allen Key (5mm) |
| Impact Resistance | 1.63 MPa | 2.01 MPa |
| Latency | < 200ms | 165-180ms |
| Activation Success | > 70% | 78% |

## Hardware Specifications
* **sEMG Sensor**: Myoware 2.0 sEMG Sensor - *Non-invasive muscle signal acquisition via a secure arm band.*
* **Microcontroller**: Teensy 4.1 - *Real-time signal processing, hysteresis logic, digital filtering, and a hierarchical state machine in a compact footprint.*
* **Control Interface**: ILI9341 Touchscreen - *Provides real-time feedback on battery life, signal strength, and system health to ensure rider confidence.*
* **Actuator**: SB-2290SG High-Torque Servo - *Translates signals into the mechanical force required to pull a standard brake lever.*
* **Power**: Venom 4000 2S LiPo Battery - *High-discharge current for consistent braking and 2+ hours of active ride time.*
* **Signal Wires**: 22AWG Shielded UL2464 - *Protects sensitive bio-signals and PWM output from interference (high SNR).*
* **Housing**: 3D Printed PETG - *Rugged, impact-resistant enclosure designed to protect internals during high-intensity trail riding.*
* **Bike Clamps**: Motorcycle Mounting Brackets - *Heavy-duty hardware chosen to prevent slip under the servo’s peak torque.*
</details>

<details>
<summary><b>View System Architecture Overview</b></summary>    
 
## System Architecture Overview
Figure 1: Level 0 System Context (As-Built) - Refined architectural overview post defense to demonstrate the highest level system overview focusing on the IPO model.
<img width="1095" height="900" alt="v1 0 System Overview (3)" src="https://github.com/user-attachments/assets/e2d98aab-598f-4c3a-a519-d3b4d58133bb" />
Diagram illustrating the signal path from sEMG acquisition (user's bicep) to mechanical brake activation, including power and system telemetry. 

The MABPA architecture is divided into three integrated subsystems: Electrical, Mechanical, and Embedded.
* **Signal Acquisition**: A surface electromyography (sEMG) biosensor captures muscle activity from the user’s bicep. It outputs a filtered "envelope signal"—a smoothed representation of raw muscle contractions—directly to the microcontroller.
* **Processing & Command**: The Teensy 4.1 Microcontroller processes this analog signal in real-time. When a "flex" is detected above the calibrated threshold, the embedded logic triggers a high-torque servo motor to engage the bike’s hand brake. When the user relaxes, the system sends a "release" command to return the servo to its resting position.
* **User Interface & Power**: A touchscreen dashboard provides live monitoring, including battery life, a brake-force bar graph, and current braking status. The system is powered by a rechargeable 7.4V LiPo battery.
* **Protection**: All sensitive electronics are secured within 3D-printed PETG housings (indicated by green dashed lines), providing rugged impact resistance for trail riding.
</details>

<details>
<summary><b>View Software & Systems Architecture</b></summary>  
 
## Software & Systems Architecture
As the Embedded Lead, I was responsible for the full lifecycle of our control system, from hardware selection and component validation to the final hierarchical state machine (HSM).
* **Component Selection**: Developed **Criteria Matrices** to evaluate hardware and software based on performance benchmarks:
  * **Microcontroller**: Optimized for **clock speed, ADC resolution, PWM precision,** and **communication channels (SPI)**.
  * **UI Display**: Evaluated **viewing angle, sunlight visibility,** and **durability** in rough terrain.
  * **System Power**: Compared **power consumption** and **footprint** to balance battery life with a compact bike-frame compatible form factor.
  * **Development Environment**: Selected VS Code with PlatformIO to leverage **advanced library management** and **low-level hardware access**.
 

*  **Modular Validation**: Developed a suite of **Peripheral Test Harnesses** to isolate and calibrate the sEMG sensor, high-torque servo, touchscreen and battery.
  * **sEMG Placement Optimization**: Conducted iterative testing on bicep positioning to **maximize signal reliability** and **minimize false-positive** flex triggers.
  * **Signal Characterization**: Verified **sEMG noise floors** and **ADC scaling** to ensure reliable trigger thresholds.
  * **Actuator Stress-Testing**: Validated **servo PWM response times** and **current draw** under simulated braking loads.

* **Firmware Architecture**: Designed a Hierarchical State Machine (HSM) composed of nested state machines to guarantee deterministic, safety-critical performance.
* Top-Level System HSM: The root controller managing global transitions between three primary states: **Initialization**, **System Ready**, and **Failure**. It is the gatekeeper, interpreting status codes from sub-systems before allowing operation.
  * StartUp FSM (finite state machine): Orchestrates **hardware self-tests** (servo actuation, battery verification, and peripheral connectivity) while simultaneously managing the UI control screen.
    * MuscleSensor FSM: A dedicated **calibration sub-routine** guiding the biker through armband placement and "test flexes" to **confirm expected bio-signals** before riding.
  * SystemRunning FSM: The primary operational loop that manages **real-time transitions** between **Brake-Release** and **Brake-Activate** states while maintaining **continuous battery health monitoring**.
 

![Top-Level](https://github.com/user-attachments/assets/fa2a352e-88e1-4cd9-894b-94c75f2b2596)
Figure Description: Top-level System HSM diagram with three states.
The **System HSM** serves as the primary controller, managing transitions between three global states: `INIT`, `ERROR`, and `READY`.
* **INIT (Initialization)**: Upon power-on, the system executes `startUpFSM()`. This nested routine conducts hardware self-tests (servo, battery, and peripheral connectivity).
* **ERROR (Safety Lockout)**: If `startUpFSM()` detects a critical hardware fault, it returns failure code `INIT_ERROR`, triggering a transition to the `ERROR` state. The system enters a terminal lockout to ensure rider safety, requiring a hard power cycle to reset.
* **READY (Operational)**: Once `startUpFSM()` returns a success flag `INIT_SUCCESS`, the system transitions to the `READY` state. In this mode, the `systemRunningFSM()` takes control to manage real-time sEMG processing and mechanical braking.


![StartUp FSM Flow](https://github.com/user-attachments/assets/d1476845-4d89-4c33-be71-f6ac2ee876b0)
Figure Description: StartUp FSM Flow diagram on the left with control screen captures on the right.
The **StartUp FSM** manages system health checks prior to operation, transitioning between five primary states: `DRAW_LOGO`, `INIT_BATTERY`, `LOW_BATTERY`, `INIT_SERVO`, and `INIT_MUSCLE`.
* **DRAW_LOGO**: Renders the MABPA splash screen. This state **masks background health checks** to provide a **seamless user experience** without delaying system initialization.
* **INIT_BATTERY**: The `updateBatteryReading()` function evaluates initial voltage.
  * Critical Fault: If voltage is < 7.4V, the system returns `INIT_ERROR` and triggers a critical battery lockout screen.
  * Warning: If voltage is between 7.4V and 7.9V, the system enters `LOW_BATTERY`, prompting a user override via touchscreen to proceed.
  * Nominal: If voltage is > 7.9V, the system proceeds directly to `INIT_SERVO`.
* **INIT_SERVO**: The `initServo()` function verifies actuator connectivity and response. A failure here returns `INIT_ERROR`, transitioning the Top-Level HSM to the `ERROR` state and displaying a servo-fault message.
* **INIT_MUSCLE**: The final initialization phase where the `muscleFSM()` is called to calibrate the biosensor and confirm valid sEMG signal acquisition.


![Muscle FSM Flow](https://github.com/user-attachments/assets/d0390a6a-d771-4445-83ef-5313a304da7d)
Figure Description: Muscle FSM Flow diagram on the left with control screen captures on the right.
The Muscle FSM is a sub-routine of the StartUp FSM designed to calibrate the sEMG biosensor and verify signal integrity through five states: `RUN_PLACEMENT_PROMT`, `RUN_TIMER_PROMT`, `COLLECT_EMG_DATA`, `EMG_DATA_ERROR`, and `EMG_DATA_VALID`.
* **RUN_PLACEMENT_PROMT**: Initializes the MyoWare 2.0 sensor and prompts the rider to secure the armband. The system waits for a `userReady` touchscreen interrupt to transition.
* **RUN_TIMER_PROMPT**: Executes `uscleFlexPrompt()`, instructing the rider how to perform test flexes. Tapping the screen triggers `resetEMGdata()`, clearing the buffer and starting a 15-second hardware timer.
* **COLLECT_EMG_DATA**: Performs real-time data acquisition. The system tracks the peak-to-peak voltage (Max/Min) of the sEMG signal while displaying a live countdown on the control screen.
  * **Validation Logic**: Upon timer expiration, the system calculates the Dynamic Range (`ΔV = Max - Min`):
    * **EMG_DATA_ERROR**: If `ΔV < 0.65V`, the signal is flagged as insufficient. The UI prompts for sensor repositioning and loops back to the timer prompt.
    * **EMG_DATA_VALID**: If `ΔV > 0.65V`, the signal is verified. Upon a final user tap, the FSM returns the `EMG_DONE` flag to the StartUp FSM, successfully concluding the calibration. This shunts the `INIT_SUCESS` flag up to the System HSM, causing the transition from `INIT` to `READY`.
   

![System Running FSM](https://github.com/user-attachments/assets/b6da94a3-8c18-4963-9eaf-f2e806617a0f)
Figure Description: SystemRunning FSM flow diagram (left) with corresponding UI control screen captures (right).
Once `INIT_SUCCESS` is received, the Top-Level HSM enters the `READY` state and hands control to the `SystemRunningFSM()`. This machine operates within a high-speed control loop synchronized to a 1500Hz Interrupt Service Routine (ISR) managing three states: `RUN_INIT`, `RELEASE_BRAKE`, and `ENGAGE BRAKE`.
* **High-Frequency Data Acquisition**: The `isSampleReady()` flag triggers at 1500Hz to process raw sensor sEMG data, adhering to the Nyquist Sampling Theorem. To optimize processor overhead, the brake-force bargraph and UI refresh rate are decouples from the sEMG polling, set at 35Hz.
* **RUN_INIT**: The entry state for operation. It executes `updateBatteryScreen()` and initializes the control screen to the Brake Released status.
* **RELEASE_BRAKE** (Idle State): The default operational state. To minimize overhead and battery concumption, battery updates are polled on a 5-second interval while the system waits for a brake trigger.
* **Threshold Logic & Actuation**: The system utilizes hysteresis to prevent "chatter" or accidental disengagement:
  * **Engagement**: When filtered sEMG levels exceed the `engageBrakeThreshold`, the FSM transitions to `ENGAGE_BRAKE`, executing the `brakeServo()` function to pull the lever.
  * **Disengagement**: The system remains engaged until the muscle signal drops below the `releaseBrakeThreshold`, triggering the `releaseServo()` function and returning to the idle state.
  * **Background Monitoring**: The FSM maintains a continuous check for battery health, ensuring the rider is notified of power drops during active use.
 
 </details>






