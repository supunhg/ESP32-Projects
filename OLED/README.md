# HyperCube - 4D Hypercube Visualization 🔮

A mesmerizing 4D hypercube (tesseract) visualization with particle effects for ESP32 and OLED displays. This project demonstrates advanced mathematical concepts including 4D rotation, projection, and real-time graphics rendering on a small OLED screen.

## 🎯 Project Overview

This MicroPython script creates a rotating 4D hypercube visualization on a 128x64 OLED display. The hypercube rotates through multiple dimensions simultaneously while surrounded by animated particles and occasional glitch effects for a cyberpunk aesthetic.

## 🔬 Mathematical Concepts

### 4D Hypercube (Tesseract)
- **16 vertices** in 4-dimensional space
- **32 edges** connecting vertices that differ by exactly one coordinate
- **8 cubic cells** (3D cubes that form the hypercube's boundary)

### Projection Process
1. **4D → 3D**: Project from 4D space to 3D using perspective projection
2. **3D → 2D**: Project from 3D space to 2D screen coordinates
3. **Multi-axis Rotation**: Simultaneous rotation around 6 different 4D planes

## 🛠️ Hardware Requirements

### Core Components
- **ESP32 Development Board** (or any MicroPython-compatible board)
- **128x64 OLED Display** (SSD1306, I2C interface)
- **Jumper Wires** for connections

### Optional Components
- **Breadboard** for prototyping
- **Pull-up Resistors** (usually not needed for I2C)

## 🔌 Wiring Diagram

### I2C OLED Display (SSD1306)
| OLED Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC      | 3.3V      | Power supply |
| GND      | GND       | Ground |
| SDA      | GPIO 21   | I2C Data line |
| SCL      | GPIO 22   | I2C Clock line |

**Simple Connection:**
```
ESP32    OLED
----     ----
3.3V  -> VCC
GND   -> GND
21    -> SDA
22    -> SCL
```

## 🚀 Getting Started

### 1. Hardware Setup
1. Connect the OLED display to your ESP32 according to the wiring diagram
2. Ensure stable power supply to both devices
3. Double-check I2C connections (SDA/SCL)

### 2. Software Installation

#### Install MicroPython
1. **Download MicroPython** firmware for ESP32
2. **Flash the firmware** using esptool or Thonny IDE
3. **Connect to the device** via USB

#### Install Required Libraries
```python
# Upload ssd1306.py library to your ESP32
# This library is available from the MicroPython repository
# or can be installed via upip (if available)
```

#### Upload the Script
1. Copy `HyperCube.py` to your ESP32's filesystem
2. Run the script using your MicroPython IDE or REPL

### 3. Running the Visualization
```python
# Execute in MicroPython REPL or run as main script
exec(open('HyperCube.py').read())
```

## 🎮 Features

### Visual Effects
- **4D Hypercube**: Real-time rotating tesseract visualization
- **Particle System**: 20 animated particles with physics
- **Glitch Effects**: Random visual artifacts for cyberpunk aesthetic
- **FPS Counter**: Real-time performance monitoring

### Technical Features
- **Multi-axis Rotation**: 6 simultaneous rotation planes (XY, XZ, XW, YZ, YW, ZW)
- **Perspective Projection**: Realistic 4D→3D→2D projection
- **Particle Physics**: Individual particle movement and lifecycle
- **Performance Optimization**: Efficient rendering for small displays

## 🔧 Customization Options

### Modify Rotation Speed
```python
# In the main loop, change the angle increment
angle += 0.02  # Default speed
angle += 0.05  # Faster rotation
angle += 0.01  # Slower rotation
```

### Adjust Particle Count
```python
# Change the number of particles
particles = []
for _ in range(50):  # Increase from 20 to 50 particles
    # ... particle initialization
```

### Modify Projection Distance
```python
# Change projection distance for different perspective effects
projected_3d = [project_4d_to_3d(v, 5) for v in rotated]  # Further distance
projected_2d = [project_3d_to_2d(p, 2) for p in projected_3d]  # Closer distance
```

### Scale the Hypercube
```python
# Modify the scaling factor in the drawing code
x1, y1 = int(64 + x1 * 40), int(32 + y1 * 40)  # Larger hypercube
x2, y2 = int(64 + x2 * 40), int(32 + y2 * 40)  # Larger hypercube
```

### Customize Glitch Effects
```python
# Change glitch probability and intensity
if random.random() < 0.1:  # More frequent glitches
    for _ in range(20):     # More intense glitches
        # ... glitch code
```

## 📊 Performance

### Expected Performance
- **Frame Rate**: ~15-30 FPS on ESP32
- **Memory Usage**: Minimal (uses basic Python lists)
- **CPU Usage**: Moderate (mathematical calculations)

### Optimization Tips
- **Reduce Particles**: Lower particle count for better performance
- **Simplify Projections**: Use integer math where possible
- **Adjust Sleep**: Modify `time.sleep(0.01)` for different frame rates

## 🐛 Troubleshooting

### Common Issues

#### Display Not Working
- **Check I2C Address**: Default is 0x3C, some displays use 0x3D
- **Verify Connections**: Ensure SDA/SCL are properly connected
- **Power Supply**: Make sure display gets stable 3.3V

#### Low Frame Rate
- **Reduce Complexity**: Lower particle count or simplify effects
- **Check CPU Load**: Monitor system performance
- **Power Issues**: Ensure adequate power supply

#### MicroPython Errors
- **Memory Errors**: Reduce particle count or simplify calculations
- **Import Errors**: Ensure ssd1306.py library is uploaded
- **Syntax Errors**: Check MicroPython compatibility

#### Mathematical Issues
- **Projection Artifacts**: Adjust projection distances
- **Rotation Problems**: Verify rotation matrix calculations
- **Coordinate Overflow**: Check boundary conditions

## 🎓 Educational Value

This project teaches:

### Mathematics
- **4D Geometry**: Understanding tesseracts and hypercubes
- **Linear Algebra**: Rotation matrices and transformations
- **Projective Geometry**: 4D→3D→2D projection principles
- **Trigonometry**: Sin/cos functions for rotation

### Programming
- **Real-time Graphics**: Frame-based animation loops
- **3D Mathematics**: Coordinate transformations
- **Performance Optimization**: Efficient rendering techniques
- **Object-Oriented Design**: Particle system implementation

### Physics
- **Particle Systems**: Simple physics simulation
- **Perspective Projection**: Camera and viewing concepts
- **Frame Rate**: Real-time system constraints

## 🔬 Advanced Modifications

### Add Color Support
```python
# For color OLED displays, modify pixel calls
oled.pixel(x, y, color_value)  # Use different colors for different elements
```

### Interactive Controls
```python
# Add button inputs to control rotation speed or effects
button = machine.Pin(0, machine.Pin.IN, machine.Pin.PULL_UP)
if not button.value():
    angle += 0.1  # Faster rotation when button pressed
```

### Data Logging
```python
# Log performance metrics
import json
performance_data = {
    'fps': fps,
    'timestamp': time.ticks_ms(),
    'particle_count': len(particles)
}
```

### Network Visualization
```python
# Send visualization data over WiFi for monitoring
import network
import socket
# ... networking code to stream data
```

## 🔗 Related Concepts

### Mathematical Background
- **Tesseract**: 4-dimensional analog of a cube
- **Hypersphere**: 4D sphere for comparison
- **Klein Bottle**: Non-orientable 4D surface
- **Quaternions**: Alternative 4D rotation representation

### Graphics Programming
- **Ray Tracing**: Advanced 3D rendering techniques
- **Shader Programming**: GPU-based graphics effects
- **Matrix Transformations**: Linear algebra in graphics
- **Perspective Cameras**: 3D viewing systems

## 🤝 Contributing

Contributions and improvements welcome:
- **Performance Optimizations**: Faster rendering algorithms
- **Visual Effects**: New particle systems or effects
- **Mathematical Accuracy**: Improved projection methods
- **Documentation**: Better explanations of concepts
- **Hardware Support**: Compatibility with different displays

## 📚 Further Reading

- **"Flatland" by Edwin Abbott**: Introduction to dimensional thinking
- **"The Fourth Dimension" by Charles Howard Hinton**: Classical 4D mathematics
- **Linear Algebra Textbooks**: Matrix operations and transformations
- **Computer Graphics Resources**: 3D projection and rendering techniques

## ⚠️ Notes

- **Performance Varies**: Frame rate depends on ESP32 model and conditions
- **Educational Purpose**: Primarily for learning 4D mathematics concepts
- **Power Consumption**: Continuous operation may drain battery quickly
- **Mathematical Approximations**: Uses floating-point calculations

---

**Experience the Fourth Dimension!** 🔮✨

*Part of the ESP32 Projects Library - Exploring advanced mathematical visualization and real-time graphics programming.*
