# MABPA System and Subsystem Schematics
> Undergoing a transition from Fritzing to KiCad to meet professional documentation standards.

## Power Distribution
Our initial [power distribution design](https://github.com/ashthemech/MABPA/blob/main/documentation/schematics/power_management_schematic_v1.0.pdf) faced a critical challenge during the Baskin Engineering Senior Design Showcase. While demonstrating the MABPA system, we observed a 5%–8% transient voltage sag triggered by the 10A inrush current of our braking servo. We diagnosed this as a lack of adequate decoupling and filtering, and with the final design defense looming, we thad to move away from a perfect "theoretical design" to high-stakes troubleshooting and quick implementation.

### The Solution: A Tiered Decoupling Network
Rather than applying a single bulk capacitor, Ashton engineered a hardware-level fix for our protoboard implementation:
* **Bulk Storage:** High-capacitance electrolytic capacitors (1000uF and 100uF) to serve as a local reservoir for the servo and the MCU during peak load.
* **High-Frequency Suppression:** Low-ESR ceramic capacitors (1uF and 0.1uF)positioned at load entry points to filter switching noise from the buck regulator and any noise in the battery line to the servo.

To ensure the "noisy" return currents from the servo didn't interfere with the sensitive analog muscle data, Ashton also implemented a star-grounding point at the regulator’s ground pin by using a ground wire.

**The Result:** [This design](https://github.com/ashthemech/MABPA/blob/main/documentation/schematics/power_management_schematic_v2.0.pdf) stabilized the power rail, reducing a 40mV transient drop to a negligible <5mV blip, maintaining a stable reference for the control architecture during peak torque events.
