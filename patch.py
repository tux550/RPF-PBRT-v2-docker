import os
import re

# SCRIPT TO PATCH OPENEXR HEADERS FOR LINUX


def replace_declspec_with_attribute(file_path):
    # Open the file
    with open(file_path, 'r') as file:
        content = file.read()

    # Regex pattern to find `#define SOMETHING __declspec(dllimport)`
    pattern = r'#define\s+(\w+)\s+__declspec\((dllimport)\)'

    # Replace with the Linux equivalent for shared libraries
    content = re.sub(pattern, r'#define \1 __attribute__((visibility("default")))', content)

    # Write the modified content back to the file
    with open(file_path, 'w') as file:
        file.write(content)

def process_directory(directory):
    # Walk through all files in the directory
    for root, dirs, files in os.walk(directory):
        for file in files:
            # Process only .h and .cpp files
            if file.endswith('.h') or file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                replace_declspec_with_attribute(file_path)
                print(f"Processed: {file_path}")

if __name__ == '__main__':
    directory = "/usr/include/OpenEXR"
    process_directory(directory)
    print("Replacement complete.")
