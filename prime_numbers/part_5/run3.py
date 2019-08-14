import matplotlib.pyplot as plt
import numpy as np
import math

with open("./result_python.txt") as f:
    content = f.readlines()

content = [x.strip() for x in content]

my_dpi = 96
count = 1
max_x = 0
max_y = 0

for c in content:
    c = c.split(",")
    x = int(c[0])
    r = int(c[1])

    if max_x < (x + r) * 1.05:
        max_x = int(round((x + r) * 1.05, 0))
        
    if max_y < (r * 1.05):
        max_y = int(round(r * 1.05, 0))

x = []
for i in range(0, max_x):
    x.append(i)

y = []
for i in range(0, max_y):
    y.append(i)
    

fig = plt.figure(figsize=(max_x, max_y), dpi=96)

ax = fig.add_subplot(111)
ax.axhline(y=0, linewidth=2, color='blue')
ax.axvline(x=0, linewidth=2, color='blue')
ax.set_xticks(x)
ax.set_yticks(y)
ax.set_xlim(0, max_x)
ax.set_ylim(0, max_y)

points = []

for c in content:
    print("Generating " + str(count) + " / " + str(len(content)))
    
    c = c.split(",")
    x = int(c[0])
    r = int(c[1])
    
    circle = plt.Circle((x, 0), r, linewidth=1, fill=False, edgecolor='red')
    ax.add_artist(circle)
    plt.scatter(x, r, s=1000, color="black")
    
    points.append({"x": x, "r": r})
    
    count = count + 1
    
for point in points:
    x1 = point["x"]
    y1 = point["r"]
    
    for point2 in points:
        x2 = point2["x"]
        y2 = point2["r"]
        
        if x1 > x2:
            if abs(x1 - x2) == abs(y1 - y2):
                plt.plot([x1,x2], [y1,y2], linewidth=3, color="black")
        

print("Saving...")


plt.savefig("out3.png", bbox_inches="tight")
