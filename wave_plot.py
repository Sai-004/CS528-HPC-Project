import matplotlib.pyplot as plt
import numpy as np

greedy_results = []
brute_force_results = []

# Read data from the file
with open("data.txt", "r") as file:
    lines = file.readlines()
    for i in range(0, len(lines), 3):
        greedy_result = float(lines[i+1].split(": ")[1])
        brute_force_result = float(lines[i+2].split(": ")[1])
        greedy_results.append(greedy_result)
        brute_force_results.append(brute_force_result)

# Plot the data
x = np.arange(1, len(greedy_results)+1)

# Create subplots
fig, axes = plt.subplots(1, 2, figsize=(14, 6))

# Plot for Greedy
axes[0].plot(x, greedy_results, color='blue', label='Greedy')
axes[0].set_title('Greedy Results')
axes[0].set_xlabel('Test Case')
axes[0].set_ylabel('Result')

# Plot for Brute Force
axes[1].plot(x, brute_force_results, color='red', label='Brute Force')
axes[1].set_title('Brute Force Results')
axes[1].set_xlabel('Test Case')
axes[1].set_ylabel('Result')

# Show legend
axes[0].legend()
axes[1].legend()

plt.tight_layout()  # Adjust layout to prevent overlap
plt.show()
