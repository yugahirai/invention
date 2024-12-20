import matplotlib.pyplot as plt
import numpy as np
import sys

# Get the file path from the user command
if len(sys.argv) != 2:
    print("Usage: python python_plot.py <file_path>")
    sys.exit(1)

file_path = sys.argv[1]

# Read the contents of the file
with open(file_path, 'r') as file:
    lines = file.readlines()

values = [int(line.strip()) for line in lines]
x_values = list(range(1, len(values) + 1))

# Calculate the average and sum
average_value = np.mean(values)
sum_value = np.sum(values)

# Set the figure size with a larger width
plt.figure(figsize=(30, 6))  # Width is increased to 30, height is 6
plt.plot(x_values, values, linestyle='-', color='b', label='Distance')
plt.axhline(y=average_value, color='r', linestyle='--', label=f'Average = {average_value:.2f}')

# Add the sum as text on the plot
plt.text(len(x_values) * 0.8, max(values) * 0.9, f'Sum = {sum_value}', fontsize=12, color='g')

plt.xlabel('INSTRUCTION NUMBER')
plt.ylabel('DISTANCE')
plt.title('Graph of Distance')
plt.grid(True)
plt.legend()

# Save the plot as an image file
output_image = 'distance_plot.png'
plt.savefig(output_image)
plt.close()

print(f"Graph with average and sum saved as {output_image}")