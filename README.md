# (C++) Python User Interface Library

![Build](https://github.com/0lru/p3ui/workflows/Build/badge.svg)
[![PyPI version](https://badge.fury.io/py/p3ui.svg)](https://badge.fury.io/py/p3ui)

This project aims at fast prototyping and development of graphical
applications. It combines [Dear ImGui](https://github.com/ocornut/imgui),
[Skia python binding](https://github.com/kyamagu/skia-python) and
related projects like [ImPlot](https://github.com/epezent/implot)
in a C++ wrapper.
This wrapper can be used from within Python though [Pybind11](https://github.com/pybind/pybind11).

[ImGui Elements](python/gallery) |  [Matplotlib Integration](demos/matplotlib)
:-------------------------|:-------------------------
![widgets](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr0.png)  |  ![matplotlib](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr1.png)|
**[ImPlot Integration](demos/gallery)** |  **[Layout System](demos/gallery)**|
![widgets](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr2.png)  |  ![matplotlib](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr3.png)
**[SVG](demos/canvas)** | |
![svg](https://raw.githubusercontent.com/0lru/p3ui/main/doc/scr4.png) | |

# Installation

> pip install p3ui

# Hello World!

```python
#
# run, Window, Text, Row, Justification, Alignment, em ..
from p3ui import *


async def main():
    window = Window(title='Hello World!')
    window.user_interface.content = Row(
        justify_content=Justification.SpaceAround,
        align_items=Alignment.Center,
        padding=(1 | em, 1 | em),
        children=[
            Text(label='Hello'),
            Text(label='World!')
        ])
    await window.closed


run(main())

```

Note that all elements belong to a single thread. They can only get modified from within this thread. All callbacks and even the renderer will execute in this single thread at the time of writing. Although the event loop does not support all the features of asyncio, it can be utilized to 
[create](https://docs.python.org/3/library/asyncio-task.html#asyncio.create_task) 
or 
[schedule](https://docs.python.org/3/library/asyncio-eventloop.html#asyncio.loop.call_soon)
tasks and for multithreading through the 
[run_in_executor](https://docs.python.org/3/library/asyncio-eventloop.html#asyncio.loop.run_in_executor)
function. A few [code examples](demos/) are provided which are also linked in the section above.
