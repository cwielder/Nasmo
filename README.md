# 🌌 Overview
**Nasmo** is a high-performance 3D game engine written in C++. It offers a cross-platform API for writing 3D interactive applications and games complete with graphical and audio capabilities.

# 🚀 Features
* **Clean, easy-to-use API**: Modular, object-oriented design allows for simple usage of the engine components.
* **Performant**: Optimized code and libraries along with smart techniques such as instancing results in high performance even on low-end devices.
* **Graphics**: Modern high-fidelity graphics and lighting is available with a deferred physically-based renderer.
* **Audio**: Low latency spatial audio for music and sound effects allows for an immersive experience.
* **UI**: Robust anchor based UI system that allows you to make interactive animated interfaces using elements.
* **Flexibility**: Every aspect of the engine has been built with flexibility and extensibility in mind, so manual control has been provided for every system to cater for advanced use cases. 

# 📚 Documentation
Coming soon.

# 🔨 Building
This project is built upon the [Premake5](https://premake.github.io/) metabuild system, meaning that it can compile using native build systems depending on the host. Currently, Windows and Linux are supported, and build systems for Visual Studio and GNU Make can be generated for each respectively by running the corresponding setup script found in the `scripts` directory at the root of the repository.

## Steps
* Clone the repository:
```
git clone https://github.com/cwielder/Nasmo.git
cd Nasmo
```
* Generate build files:
  * Windows (Visual Studio):
    ```
    "./scripts/setupVS.bat"
    ```
    Open the resulting `Nasmo.sln` file in Visual Studio 2022+
  * Linux (GNU Make):
    ```
    "./scripts/setupMake.sh"
    make
    ```

# 📃 License
All code in the `Nasmo` repository has been made available under the [MIT License](https://github.com/cwielder/Nasmo/blob/main/LICENSE.txt).
