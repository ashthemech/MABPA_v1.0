# MABPA Physical Integration
Bringing the Muscle Activated Braking Prosthetic Attachment (MABPA) from a bench circuit to an operable prototype required balancing high-torque mechanical demands with sensitive embedded electronics.

## Embedded Control & Housing
* **Protoboard Implementation** : The [central control unit](https://github.com/ashthemech/MABPA/blob/main/documentation/implementation/MABPA%20Protoboard%20and%20Housing%20Implementation.jpg) features a hand-soldered protoboard containing the Teensy 4.1, the decoupling network, resettable fuses, battery level detector, and the telemetry screen connections.
* **Space Constraints**: The board was designed and soldered within strict mounting constraints to fit into the 3D-printed housing, mounted to a slidable sheet for easy debugging access.
* **Vibration Mitigation**: To protect the logic from road noise, rubber stoppers were repurposed from a defunct screen to serve as improvised vibration isolators.

## Mechanical Rigging & Safety
* **High-Torque Mounting**: Standard bicycle clamps were insufficient for the high-torque activation required for emergency braking. Benjamin selected and integrated [heavy-duty motorcycle clamps](https://github.com/ashthemech/MABPA/blob/main/documentation/implementation/MABPA%20Underside%20Mount.jpg) to ensure the servo remained stationary under load.
* **Serviceability**: All mounting hardware uses a standard 5mm Allen key, ensuring the system can be adjusted or serviced in the field using common cycling multi-tools.
* **Brake Actuation**: The servo connects to the brake lever via a ball joint and "hook" interface secured by a hose clamp.
* **Failsafe Design**: The hook is geometrically locked under servo tension, meaning it cannot be dislodged by mechanical force during braking; it requires intentional manual removal by the rider.

## Power Management
* **Battery Integration**: The left chassis of the housing features a removable access panel secured by screws, allowing the rider to quickly swap the 5000mAh LiPo battery in the field or to recharge.
* **Rugged Interconnects**:
  * Main Power Rail: XT60H connectors were used for the battery interface to provide a high-current, anti-vibrational connection capable of handling the servo inrush.
  * Logic Interface: JST MXN-2 connectors were used to interface with the protoboard for a secure mechanical lock against road vibration.
