# Computer Graphics OpenGL Scene (C++ / Code::Blocks)

A modular **Computer Graphics project** built with **C++ and OpenGL (GLUT)**. The project demonstrates 2D scene composition, animation, and keyboard interaction using an orthographic projection. 
Each visual element is structured into its own drawing function/class to keep the code clean, maintainable, and extensible.


## Project Overview

This project renders an animated graphical scene containing multiple objects such as natural elements, vehicles, and environmental effects. 
It follows good graphics-programming practices by separating responsibilities across functions and maintaining a clear rendering pipeline.

The project is configured as a **Code::Blocks (.cbp)** project and is suitable for academic submissions, demos, and learning OpenGL fundamentals.


##  Features

* **2D Scene Rendering** using OpenGL
* **Modular Object Design** (each object drawn in its own function)
* **Keyboard Interaction** for animations and effects
* **Smooth Animations** using GLUT timer/idle functions
* **Special Effects** (rain, bubbles, movement, etc.)
* **Orthographic Projection** for precise 2D control

## Technologies Used

* **Language:** C++
* **Graphics Library:** OpenGL (GLUT)
* **IDE:** Code::Blocks
* **Compiler:** GCC / MinGW
* **Platform:** Windows


## Project Structure

| File / Folder       | Description                                 |
|---------------------|---------------------------------------------|
| `Try.cbp`           | Code::Blocks project file                   |
| `main.cpp`          | Main OpenGL source file                     |
| `README.md`         | Project documentation                       |
| 'PDF for details'   | Project report for more details             |


> All drawable elements follow a consistent naming convention such as:
> `draw_sky()`, `draw_sun()`, `draw_vehicle()`, etc.



## Controls & Keyboard Interaction

The application supports both **keyboard and mouse interaction** to control animations and visual effects within the OpenGL scene.

### Keyboard Controls

| Key              | Description                                                  |
| ---------------- | ------------------------------------------------------------ |
| `1`              | Starts the airplane animation and moves it across the sky    |
| `R` / `r`        | Toggles the rain effect with bubble formation                |
| `D` / `d`        | Switches the scene to **Day mode**                           |
| `N` / `n`        | Switches the scene to **Night mode** (moon and stars appear) |
| `↑` (Up Arrow)   | Moves the selected object upward                             |
| `↓` (Down Arrow) | Moves the selected object downward                           |
| `+`              | Increases zoom level / scales the scene up                   |
| `-`              | Decreases zoom level / scales the scene down                 |
| `ESC`            | Exits the application                                        |

### Mouse Controls

| Mouse Action       | Function                                               |
|--------------------|--------------------------------------------------------|
| Left Mouse Click   | Raises the man’s hand and stops the yellow bus        |
| Left Mouse Click   | Changes bird direction (left ↔ right)                 |
| Mouse Wheel Click  | Moves the hot air balloon up and down                 |


### Notes

* Keyboard and mouse inputs are handled using **GLUT callback functions**.
* Animations update continuously once activated.
* Day and Night modes dynamically modify scene colors and lighting effects.

*(Controls may vary slightly based on implementation)*

---

## How to Build & Run

### Prerequisites

* Code::Blocks (with MinGW)
* OpenGL & GLUT installed

## Learning Outcomes

* Understanding OpenGL rendering pipeline
* Using `gluOrtho2D()` for 2D graphics
* Handling keyboard events in GLUT
* Creating animations using transformations
* Writing clean, modular graphics code

---

## Screenshots


### Day Mode
<img width="1920" height="1080" alt="day_scene" src="https://github.com/user-attachments/assets/25a3d8e2-54d6-4212-a6b3-9dfae972e7e9" />
### Night Mode
<img width="1920" height="1080" alt="night_scene" src="https://github.com/user-attachments/assets/f088648f-7f1b-46f3-9207-7c1628038a01" />
### Rain & Bubble Effect
<img width="1920" height="1080" alt="rain_effect" src="https://github.com/user-attachments/assets/66d0ebc1-0c12-4e90-b34e-037c88a98cf9" />

---

## Author

**Showrav Ghosh**
Computer Graphics Student

## License

This project is created for **educational purposes**. You are free to use and modify it with proper attribution.

---

If you like this project, don’t forget to give it a star on GitHub!
