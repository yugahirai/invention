import matplotlib.pyplot as plt
import numpy as np

# Read the contents of the file
file_path = 'distance.txt'

with open(file_path, 'r') as file:
    lines = file.readlines()

values = [int(line.strip()) for line in lines]
x_values = list(range(1, len(values) + 1))

# Filter out the zeros and calculate the average
non_zero_values = [value for value in values if value != 0]
average_value = np.mean(non_zero_values)

# Set the figure size with a larger width
plt.figure(figsize=(30, 6))  # Width is increased to 30, height is 6
plt.plot(x_values, values, linestyle='-', color='b', label='Distance')
plt.axhline(y=average_value, color='r', linestyle='--', label=f'Average (non-zero) = {average_value:.2f}')

plt.xlabel('INSTRUCTION NUMBER')
plt.ylabel('DISTANCE')
plt.title('Graph of Distance')
plt.grid(True)
plt.legend()

# Save the plot as an image file
output_image = 'plot_distance.png'
plt.savefig(output_image)
plt.close()

print(f"Graph with average (non-zero) saved as {output_image}")