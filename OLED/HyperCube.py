import machine
import math
import time
import random
from ssd1306 import SSD1306_I2C

# Initialize OLED
i2c = machine.I2C(scl=machine.Pin(22), sda=machine.Pin(21))
oled = SSD1306_I2C(128, 64, i2c)

# Hypercube (4D cube) vertices (16 vertices in 4D space)
vertices = []
for x in [-1, 1]:
    for y in [-1, 1]:
        for z in [-1, 1]:
            for w in [-1, 1]:
                vertices.append([x, y, z, w])

# Edges connecting the vertices (32 edges in 4D space)
edges = []
for i in range(16):
    for j in range(i+1, 16):
        if sum(1 for a, b in zip(vertices[i], vertices[j]) if a != b) == 1:
            edges.append((i, j))

# 3D projection matrix
def project_4d_to_3d(point_4d, distance):
    x, y, z, w = point_4d
    factor = distance / (distance - w)
    return [x * factor, y * factor, z * factor]

def project_3d_to_2d(point_3d, distance):
    x, y, z = point_3d
    factor = distance / (distance - z)
    return [x * factor, y * factor]

# Rotation matrices for 4D
def rotate_4d(point, angle_xy, angle_xz, angle_xw, angle_yz, angle_yw, angle_zw):
    x, y, z, w = point
    
    # XY rotation
    s, c = math.sin(angle_xy), math.cos(angle_xy)
    x, y = x * c - y * s, x * s + y * c
    
    # XZ rotation
    s, c = math.sin(angle_xz), math.cos(angle_xz)
    x, z = x * c - z * s, x * s + z * c
    
    # XW rotation
    s, c = math.sin(angle_xw), math.cos(angle_xw)
    x, w = x * c - w * s, x * s + w * c
    
    # YZ rotation
    s, c = math.sin(angle_yz), math.cos(angle_yz)
    y, z = y * c - z * s, y * s + z * c
    
    # YW rotation
    s, c = math.sin(angle_yw), math.cos(angle_yw)
    y, w = y * c - w * s, y * s + w * c
    
    # ZW rotation
    s, c = math.sin(angle_zw), math.cos(angle_zw)
    z, w = z * c - w * s, z * s + w * c
    
    return [x, y, z, w]

# Particle system
particles = []
for _ in range(20):
    particles.append({
        'x': random.uniform(-2, 2),
        'y': random.uniform(-2, 2),
        'z': random.uniform(-2, 2),
        'vx': random.uniform(-0.02, 0.02),
        'vy': random.uniform(-0.02, 0.02),
        'vz': random.uniform(-0.02, 0.02),
        'life': random.randint(50, 200)
    })

# Animation loop
angle = 0
while True:
    oled.fill(0)
    
    # Update particles
    for p in particles:
        p['x'] += p['vx']
        p['y'] += p['vy']
        p['z'] += p['vz']
        p['life'] -= 1
        
        if p['life'] <= 0 or abs(p['x']) > 3 or abs(p['y']) > 3 or abs(p['z']) > 3:
            p.update({
                'x': random.uniform(-1, 1),
                'y': random.uniform(-1, 1),
                'z': random.uniform(-1, 1),
                'vx': random.uniform(-0.02, 0.02),
                'vy': random.uniform(-0.02, 0.02),
                'vz': random.uniform(-0.02, 0.02),
                'life': random.randint(50, 200)
            })
        
        # Project and draw particles
        proj_3d = project_4d_to_3d([p['x'], p['y'], p['z'], 0], 3)
        proj_2d = project_3d_to_2d(proj_3d, 3)
        x = int(64 + proj_2d[0] * 30)
        y = int(32 + proj_2d[1] * 30)
        if 0 <= x < 128 and 0 <= y < 64:
            oled.pixel(x, y, 1)
    
    # Rotate and project hypercube
    angle += 0.02
    rotated = [rotate_4d(v, angle, angle*0.7, angle*0.5, angle*1.1, angle*0.9, angle*1.3) for v in vertices]
    projected_3d = [project_4d_to_3d(v, 3) for v in rotated]
    projected_2d = [project_3d_to_2d(p, 3) for p in projected_3d]
    
    # Draw hypercube edges
    for i, j in edges:
        x1, y1 = projected_2d[i]
        x2, y2 = projected_2d[j]
        x1, y1 = int(64 + x1 * 30), int(32 + y1 * 30)
        x2, y2 = int(64 + x2 * 30), int(32 + y2 * 30)
        oled.line(x1, y1, x2, y2, 1)
    
    # Add FPS counter
    fps = int(1 / (time.ticks_ms() / 1000)) if time.ticks_ms() > 0 else 0
    oled.text(f"FPS: {fps}", 0, 0)
    
    # Add glitch effect occasionally
    if random.random() < 0.05:
        for _ in range(10):
            x = random.randint(0, 127)
            y = random.randint(0, 63)
            oled.line(x, y, x+random.randint(1,5), y+random.randint(1,5), 1)
    
    oled.show()
    time.sleep(0.01)
