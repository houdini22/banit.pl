import matplotlib.pyplot as plt
import numpy as np
import math

with open("./cmake-build-debug/data/python_data.txt") as f:
    content = f.readlines()

content = [x.strip() for x in content]

my_dpi = 96
count = 1
min = -999
max = 999

data = []

for c in content:
    c = c.split(",")
    start = int(c[0])
    end = int(c[1])
    y = count

    data.append({"start": start, "end": end, "y": y})
    count = count + 1

x = []
for i in range(min, max):
    x.append(i)

y = []
for i in range(min, max):
    y.append(i)

fig = plt.figure(figsize=(max / 12, max / 12), dpi=96)
ax = fig.add_subplot(111)
ax.axhline(y=0, linewidth=2, color='blue')
ax.axvline(x=0, linewidth=2, color='blue')
ax.set_xticks(x)
ax.set_yticks(y)

count = 1
for d in data:
    print("Generating " + str(count) + " / " + str(len(data)))

    color = np.random.rand(3,)
    ax.plot([d["start"], d["end"]], [d["y"] * 3, d["y"] * 3], linewidth=1, color=color)
    
    count = count + 1

print("Saving...")


plt.savefig("out.png")
