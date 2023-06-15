import numpy as np

# Create a dictionary with arrays
original_dict = {
    'key1': [np.random.randint(0, 10, size=(16, 16)) for _ in range(3)],
    'key2': [np.random.randint(0, 10, size=(16, 16)) for _ in range(2)]
}

# Pad each array in the dictionary to make them 32x32
padded_dict = {}
for key, value in original_dict.items():
    padded_arrays = []
    for arr in value:
        padded_array = np.pad(arr, ((8, 8), (8, 8)), mode='constant')
        padded_arrays.append(padded_array)
    padded_dict[key] = padded_arrays

# Print the original and padded dictionary
print("Original Dictionary:\n", original_dict)
print("\nPadded Dictionary:\n", padded_dict)