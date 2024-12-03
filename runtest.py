import os
import subprocess
from datetime import datetime


# Path to the PBRT executable
PBRT_EXECUTABLE = "./pbrt-v2-rpf/src/build/linux-release/pbrt"

# Folder for log files
LOG_FOLDER = "./logs"
os.makedirs(LOG_FOLDER, exist_ok=True)

# Folder for output images
OUTPUT_FOLDER = "./output"
os.makedirs(OUTPUT_FOLDER, exist_ok=True)

# Define the parameters to test
parameters_rpf = [8, 16, 32]
vanilla_rpf = [8, 16, 32, 128, 1024, 4096, 8192]  

def run_command(command, log_file):
    """Runs a shell command and redirects its output to a log file."""
    with open(log_file, "w") as log:
        process = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, text=True)
    return process.returncode


def main(results_csv, parameters, prefix):
    with open(results_csv, "w") as f:
        f.write("experiment,spp,returncode,runtime\n")

    for param in parameters:
        experiment_name = f"Conference_{param}"
        input_file = f"./scenes/{experiment_name}.pbrt"
        output_file = f"{OUTPUT_FOLDER}/{prefix}_{experiment_name}.exr"
        log_file = f"{LOG_FOLDER}/{prefix}_{experiment_name}.log"

        # Run the PBRT command
        # NOTE: outfile parameter not working as intended due to RPF implementation. 
        # Will need to manually extract the .exr files from ./scenes folder
        command = [PBRT_EXECUTABLE, "--outfile", output_file, input_file]
        print(f"Running experiment {experiment_name} with command: {' '.join(command)}")
        
        start_time = datetime.now()
        returncode = run_command(command, log_file)
        end_time = datetime.now()

        if returncode == 0:
            print(f"Experiment {experiment_name} completed successfully in {end_time-start_time}")
        else:
            print(f"Experiment {experiment_name} failed")
        
        # Save the results
        with open(results_csv, "a") as f:
            f.write(f"{experiment_name},{param},{returncode},{end_time-start_time}\n")

if __name__ == "__main__":
    
    # CASE 1: Run the experiments for the RPF integrator
    main("results_rpf.csv", parameters_rpf, "rpf")
    # CASE 2: Run the experiments for the vanilla integrator
    #main("results_vanilla.csv", parameters_vanilla, "vanilla")


