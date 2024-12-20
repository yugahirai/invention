import numpy as np
import matplotlib.pyplot as plt
import sys



# Load data from file
obj = np.loadtxt("result.txt").T

# Create figure and first subplot
plt.figure(figsize=(12, 8))
plt.subplot(2, 1, 1)
plt.plot(obj[0], obj[2])
plt.xlabel("Time [code beat]", fontsize=12)
plt.ylabel("Executed instructions", fontsize=12)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.xlim(0, len(obj[0]))
plt.ylim(0, max(obj[2]))
plt.grid(which="major", color="black", linestyle="-", alpha=0.2)
plt.grid(which="minor", color="black", linestyle="-", alpha=0.2)

# Create second subplot
plt.subplot(2, 1, 2)
plt.plot(obj[0], obj[3], label="generated magic states")
plt.plot(obj[0], obj[4], label="stocked magic states")
plt.xlabel("Time [code beat]", fontsize=12)
plt.ylabel("Magic state count", fontsize=12)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.xlim(0, len(obj[0]))
plt.ylim(0, max(max(obj[3]), max(obj[4])))
plt.grid(which="major", color="black", linestyle="-", alpha=0.2)
plt.grid(which="minor", color="black", linestyle="-", alpha=0.2)
plt.legend(fontsize=12)

# Adjust layout and save the figure as a file
plt.tight_layout()

# Save the plot as an image file instead of showing it
output_image = 'plot_magic.png'
plt.savefig(output_image)
plt.close()

print(f"Graph saved as {output_image}")