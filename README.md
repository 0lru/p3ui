# C++, Python User Interface Library
![Build](https://github.com/0lru/p3ui/workflows/Build/badge.svg)
[![PyPI version](https://badge.fury.io/py/p3ui.svg)](https://badge.fury.io/py/p3ui)

This project aims at fast prototyping and development of graphical applications. It uses and combines [Dear ImGui](https://github.com/ocornut/imgui) and related projects like [ImPlot](https://github.com/epezent/implot) in a C++ wrapper. This wrapper is usable from within Python though [Pybind11](https://github.com/pybind/pybind11).
Additionally, [Skia](skia.org) can be used for more advanced 2D graphics. For the integration of Skia, this project supports [Skia python binding](https://github.com/kyamagu/skia-python).

[ImGui Elements](python/gallery) |  [Matplotlib Integration](demos/gallery)
:-------------------------|:-------------------------
![widgets](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr0.png)  |  ![matplotlib](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr1.png)|
**[ImPlot Integration](python/gallery)** |  **[Layout System](demos/matplotlib)**|
![widgets](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr2.png)  |  ![matplotlib](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr3.png)

Key Features and Project Goals:
* Fast prototyping and development
* Small-sized library in terms of megabytes
* A clean, asynchronous and lockfree API (to be done)
* Language- and platform agnostic GUI framework (to be done)

#  Installation

Installation with pip:

> pip install p3ui
