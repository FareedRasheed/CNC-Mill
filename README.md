# CNC Mill

This project was part of my first-year Mechatronics Engineering course (MTE100), where we designed and built a 3-axis CNC milling machine using Lego Mindstorms and RobotC (essentially C++). Aside from the mill bit (a screw my teammate found in his trunk), the entire structure was constructed using Legos and Tetrix pieces.

## Challenges & Design Iterations

Building a CNC mill with Legos has a lot of challenges, especially when designing a spindle capable of reaching 4,000 RPM. After several redesigns, we capped the speed at 2,000 RPM to prioritize safety and structural stability.

## The CNC mill operates as follows:

X & Y Axes: Two independently controlled rack-and-pinion tables move the cutting piece in two dimensions.
Z Axis: A belt-driven spindle and drill(screw) provide vertical movement for cutting. The spindle uses a 1:25 gear ratio for increased speed.
Each axis is controlled by a motor with an encoder and a limit switch, maximizing accuracy and safety given the constraints of Lego-based construction.

The CNC mill is programmed using a text file uploaded to the main controller. This file contains sequential instructions in relative coordinates, allowing the machine to execute precise cutting paths.

## Takeaways

This project was a lot of fun and a valuable learning experience in resourceful engineering and design under constraints. It challenged us to balance precision, stability, and safety while working with unconventional materials.
