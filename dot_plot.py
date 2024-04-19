import matplotlib.pyplot as plt

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
plt.figure(figsize=(10, 6))
for i in range(len(greedy_results)):
    if greedy_results[i] == brute_force_results[i]:
        plt.plot(i+1, greedy_results[i], 'go')  # Both results are equal, plot green
    else :
        plt.plot(i+1, greedy_results[i], 'ro')  # Greedy result is worse, plot red
        plt.plot(i+1, brute_force_results[i], 'bo')  # Brute Force result is better, plot blue
        
plt.title('Comparison of Greedy and Brute Force Results')
plt.xlabel('Test Case')
plt.ylabel('Result')
plt.grid(True)
plt.show()