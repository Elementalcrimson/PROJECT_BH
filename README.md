# 🌌 Black Hole Simulation (C++ + OpenGL)

A real-time black hole visualization built using **C++**, **OpenGL**, **GLFW**, and **GLEW**.
This project demonstrates basic graphics programming concepts like window creation, rendering loops, and GPU interaction.

---

## 🚀 Features

* OpenGL window rendering using GLFW
* GLEW for modern OpenGL function loading
* Real-time rendering loop
* Foundation for advanced simulations (gravitational lensing, shaders, etc.)

---

## 🛠️ Requirements

Make sure you have the following installed:

* **C++ Compiler (MinGW / g++)**
* **CMake (optional but recommended)**
* **GLFW (MinGW version)**
* **GLEW (MinGW-compatible build)**
* Windows OS

---

## 📁 Project Structure

```
BlackHoleSim/
│── main.cpp
│── README.md
│
├── libs/
│   ├── glew/
│   │   ├── include/
│   │   ├── lib/
│   │   └── bin/
│   └── glfw/
│       ├── include/
│       └── lib-mingw-w64/
```

---

## ⚙️ Build Instructions

### 1. Compile the project

Run the following command in your terminal:

```bash
g++ main.cpp ^
 -IC:/libs/glew/include -IC:/libs/glfw/include ^
 -LC:/libs/glew/lib -LC:/libs/glfw/lib-mingw-w64 ^
 -lglfw3 -lglew32 -lopengl32 -lgdi32 -o main.exe
```

---

### 2. Run the program

Make sure `glew32.dll` is in the same folder as `main.exe`, then run:

```bash
./main.exe
```

---

## 🎮 Controls

* Close the window using the top-right **X button**

---

## 🧠 Learning Goals

This project was built to learn:

* OpenGL basics
* C++ graphics setup
* Window and rendering pipelines
* Handling external libraries in C++

---

## ⚠️ Common Issues

### ❌ `cannot find -lglew32`

* Ensure `libglew32.a` exists in your GLEW `lib` folder
* Make sure you're using MinGW-compatible libraries

### ❌ `undefined reference to glfwInit`

* Check that `libglfw3.a` exists
* Verify correct `-L` paths in compile command

---

## 📌 Future Improvements

* Add shaders (GLSL)
* Implement gravitational lensing
* Add camera movement
* Improve visuals with textures

---

## 👤 Author

Elementalcrimson

---

## 📄 License

This project is for educational purposes.
