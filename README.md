# ♟️ VR Chessboard Project

## 📝 Overview
This project is the **evolution of the original OpenGL chessboard simulator**, extended to support **Virtual Reality (VR)** as part of the **Graphics course at SUPSI**.  
The goal was to enhance the existing graphics engine to enable **immersive interaction using VR headsets and hand tracking**, thus transforming a standard 3D scene into a fully explorable and interactive VR environment.

The resulting application merges **OpenGL 4.4 rendering**, **SteamVR** support, and **Leap Motion** for hand-tracked interaction, enabling users to physically pick up and move chess pieces in a realistic and intuitive way.  
Though standard chess logic (turns, legal moves, etc.) is still not enforced, the emphasis is placed on **rendering performance**, **VR immersion**, and **natural interaction** within a physically simulated 3D space.

---

## 🎨 Features

### 🖥️ Upgraded Graphics Engine
- Based on **OpenGL 4.4** for enhanced rendering quality.
- Uses **GLM** for matrix and vector operations.
- Integrated with **FreeImage** for texture loading.
- Fully VR-ready with **SteamVR** runtime compatibility.
- Optimized shaders and camera handling for stereoscopic rendering.

### 🕹️ VR Interaction & Immersion
- Compatible with **SteamVR-supported headsets** (e.g., HTC Vive, Valve Index).
- Uses **Leap Motion** for precise **hand tracking**.
- Players can grab, lift, and move chess pieces naturally using their hands.
- Support for **gesture-based controls** and collision detection.
- The chessboard and pieces respond realistically to hand motion and placement.

### ♜ Chessboard Mechanics
- Immersive 3D chessboard environment.
- Pick up and place pieces freely — no enforcement of chess rules.
- Scene lighting and shaders enhance realism in VR space.

---

## 🎮 Controls & Interactions

**Hand gestures (Leap Motion):**
- **Grab**: Pinch gesture to select and move a chess piece.
- **Drop**: Release the pinch to place a piece on the board.

---

## 📌 Requirements

To build and run the VR version of the project, ensure the following dependencies are installed:

- **OpenGL 4.4**
- **GLM**
- **FreeImage**
- **Leap Motion SDK**
- **SteamVR SDK**
- **FreeGLUT**

VR runtime (e.g., SteamVR) must be running and a supported headset connected.

---

## 👤 Authors
- Mattia Verdolin 📧 mattia.verdolin@student.supsi.ch
- Riccardo Cristallo 📧 riccardo.cristallo@student.supsi.ch
- Mattia Cainarca 📧 mattia.cainarca@student.supsi.ch

---
##📜 License
This VR chessboard application was developed for educational purposes as part of the Graphics course at SUPSI.
Redistribution or usage outside the academic scope requires prior consent from the authors.
