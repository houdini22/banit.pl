import matplotlib.pyplot as plt
import numpy as np
import math
import matplotlib.pylab as pl
import itertools

# functions
def unique(list1):
    x = np.array(list1)
    return np.unique(x)

def createColors(colors_arr):
    colors_arr = unique(colors_arr)
    color_map = {}
    count = 0
    thick = 5
    palette = itertools.cycle([
    "#000000",
    "#0000FF",
    "#8A2BE2",
    "#A52A2A",
    "#DEB887",
    "#5F9EA0",
    "#7FFF00",
    "#D2691E",
    "#6495ED",
    "#DC143C",
    "#00FFFF",
    "#00008B",
    "#008B8B",
    "#A9A9A9",
    "#006400",
    "#BDB76B",
    "#8B008B",
    "#556B2F",
    "#FF8C00",
    "#9932CC",
    "#8B0000",
    "#E9967A",
    "#8FBC8F",
    "#483D8B",
    "#2F4F4F",
    "#00CED1",
    "#9400D3",
    "#FF1493",
    "#00BFFF",
    "#696969",
    "#1E90FF",
    "#B22222",
    "#FFFAF0",
    "#228B22",
    "#FF00FF",
    "#DCDCDC",
    "#F8F8FF",
    "#FFD700",
    "#DAA520",
    "#808080",
    "#008000",
    "#ADFF2F",
    "#F0FFF0",
    "#FF69B4",
    "#CD5C5C",
    "#4B0082",
    "#FFFFF0",
    "#F0E68C",
    "#E6E6FA",
    "#FFF0F5",
    "#7CFC00",
    "#FFFACD",
    "#ADD8E6",
    "#F08080",
    "#E0FFFF",
    "#FAFAD2",
    "#90EE90",
    "#D3D3D3",
    "#FFB6C1",
    "#FFA07A",
    "#20B2AA",
    "#87CEFA",
    "#778899",
    "#B0C4DE",
    "#FFFFE0",
    "#00FF00",
    "#32CD32",
    "#FAF0E6",
    "#FF00FF",
    "#800000",
    "#66CDAA",
    "#0000CD",
    "#BA55D3",
    "#9370DB",
    "#3CB371",
    "#7B68EE",
    "#00FA9A",
    "#48D1CC",
    "#C71585",
    "#191970",
    "#F5FFFA",
    "#FFE4E1",
    "#FFE4B5",
    "#FFDEAD",
    "#000080",
    "#FDF5E6",
    "#808000",
    "#6B8E23",
    "#FFA500",
    "#FF4500",
    "#DA70D6",
    "#EEE8AA",
    "#98FB98",
    "#AFEEEE",
    "#DB7093",
    "#FFEFD5",
    "#FFDAB9",
    "#CD853F",
    "#FFC0CB",
    "#DDA0DD",
    "#B0E0E6",
    "#800080",
    "#FF0000",
    "#BC8F8F",
    "#4169E1",
    "#8B4513",
    "#FA8072",
    "#FAA460",
    "#2E8B57",
    "#FFF5EE",
    "#A0522D",
    "#C0C0C0",
    "#87CEEB",
    "#6A5ACD",
    "#708090",
    "#FFFAFA",
    "#00FF7F",
    "#4682B4",
    "#D2B48C",
    "#008080",
    "#D8BFD8",
    "#FF6347",
    "#40E0D0",
    "#EE82EE",
    "#F5DEB3",
    "#FFFFFF",
    "#F5F5F5",
    "#FFFF00",
    "#9ACD32"])
    for i in colors_arr:
        color_map[i] = {"color": next(palette), "thick": thick}
        count = count + 1

        if thick == 5:
            thick = 9
        else:
            thick = 5

    return color_map

# open
with open("./result_python.txt") as f:
    content = f.readlines()

content = [x.strip() for x in content]

# initialize
my_dpi = 96
count = 1
max_x = 0
max_y = 0
colors_arr = []
count = 0
thicks_x = []
thicks_y = []
points = []

for c in content:
    c = c.split(",")
    x = int(c[0])
    r = int(c[1])

    if max_x < (x + r) * 1.05:
        max_x = int(round((x + r) * 1.05, 0))

    if max_y < (r * 1.05):
        max_y = int(round(r * 1.05, 0))

    colors_arr.append(r)

color_map = createColors(colors_arr)

for i in range(0, max_x):
    thicks_x.append(i)
for i in range(0, max_y):
    thicks_y.append(i)

# plot
fig = plt.figure(figsize=(max_x, max_y), dpi=96)

ax = fig.add_subplot(111)
ax.axhline(y=0, linewidth=2, color='blue')
ax.axvline(x=0, linewidth=2, color='blue')
ax.set_xticks(thicks_x)
ax.set_yticks(thicks_y)
ax.set_xlim(0, max_x)
ax.set_ylim(0, max_y)

for c in content:
    print("Generating " + str(count) + " / " + str(len(content)))
    
    c = c.split(",")
    x = int(c[0])
    r = int(c[1])
    
    circle = plt.Circle((x, 0), r, linewidth=color_map[r]["thick"], fill=False, edgecolor=color_map[r]["color"], zorder=1)
    ax.add_artist(circle)
    plt.scatter(x, r, s=3000, color="black", zorder=2)
    
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
                plt.plot([x1,x2], [y1,y2], linewidth=13, color="black", zorder=2)
                
        if y1 == y2:
            plt.plot([x1,x2], [y1,y1], linewidth=13, color="black", zorder=2)
        

print("Saving...")


plt.savefig("out4.png", bbox_inches="tight")
