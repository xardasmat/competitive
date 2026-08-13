#!/usr/bin/env python3
"""
Usage: python3 check_solution.py <solution_file.cpp> <test_dir>

Arguments:
  <solution_file.cpp>: Path to the C++ source file to be tested.
  <test_dir>: Path to the directory containing test files.

This script compiles the solution and runs it against all *.in files in the test directory.
It verifies the output against *.out files (if present) and checks execution time and memory usage.
Limits can be specified in *.time (seconds) and *.memory (KB) files for each test case.
"""

import os
import sys
import subprocess
import time
import resource
import glob

def compile_solution(source_file):
    if not os.path.exists(source_file):
        print(f"Error: Source file '{source_file}' not found.")
        sys.exit(1)
    
    executable = "./solution"
    compile_cmd = ["g++", "-O3", "-std=c++17", "-o", executable, source_file]
    
    print(f"Compiling {source_file}...")
    result = subprocess.run(compile_cmd, capture_output=True, text=True)
    
    if result.returncode != 0:
        print("Compilation failed:")
        print(result.stderr)
        sys.exit(1)
    
    return executable

# returns either 'OK' or 'WA'
def compare_with_output_file(contestant_output, output_file) -> str:
    with open(output_file, 'r') as f:
        our_output = [ l.strip() for l in f.read().splitlines() ]

    contestant_output = [ l.strip() for l in contestant_output.splitlines() ]

    if contestant_output == our_output:
        return 'OK'

    return 'WA'

# returns either 'OK' or 'WA' (or '???) if output file does not exist
def judge_output(contestant_output, output_file_or_dir) -> str:

    if not os.path.exists(output_file_or_dir):
        return '???'

    if os.path.isfile(output_file_or_dir):
        return compare_with_output_file(contestant_output, output_file_or_dir)

    assert os.path.isdir(output_file_or_dir)

    results = []
    for out_file in os.scandir(output_file_or_dir):

        assert os.path.isfile(out_file)

        results.append(compare_with_output_file(contestant_output, out_file))

        if 'OK' in results:
            break

    if 'OK' in results:
        return 'OK'

    # return arbitrary non-OK value
    return results[-1]

def run_test(executable, input_file, expected_output_file, time_limit_file=None, memory_limit_file=None):
    try:
        # Read limits if available
        time_limit = float('inf')
        if time_limit_file and os.path.exists(time_limit_file):
            with open(time_limit_file, 'r') as f:
                try:
                    time_limit = float(f.read().strip())
                except ValueError:
                    pass

        memory_limit_kb = float('inf')
        if memory_limit_file and os.path.exists(memory_limit_file):
            with open(memory_limit_file, 'r') as f:
                try:
                    memory_limit_kb = float(f.read().strip())
                except ValueError:
                    pass

        with open(input_file, 'r') as infile:
            start_time = time.time()
            # Run the process
            timeout_val = time_limit if time_limit != float('inf') else 5.0
            timeout_val =  max(timeout_val * 1.5, 1.0) # Grace period

            proc = subprocess.Popen([executable], stdin=infile, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            stdout, stderr = proc.communicate(timeout=timeout_val)
            end_time = time.time()
            
            elapsed_time = end_time - start_time
            
            # Get max resident set size (memory)
            # On macOS, ru_maxrss is in bytes, on Linux it's in kilobytes.
            usage = resource.getrusage(resource.RUSAGE_CHILDREN)
            max_rss = usage.ru_maxrss
            if sys.platform == 'darwin':
                max_rss = max_rss / 1024 # Convert to KB
            
            # Check limits
            limit_status = ""
            if elapsed_time > time_limit:
                limit_status = "TLE"
            elif max_rss > memory_limit_kb:
                limit_status = "MLE"

            # Read expected output
            status = judge_output(stdout, expected_output_file)
            
            # Override status if limits exceeded
            if limit_status:
                status = limit_status
                
            return {
                "status": status,
                "time": elapsed_time,
                "memory": max_rss, # in KB
                "output": stdout,
                "error": stderr,
                "time_limit": time_limit,
                "memory_limit": memory_limit_kb
            }
            
    except subprocess.TimeoutExpired:
        if 'proc' in locals():
            proc.kill()
        return {"status": "TLE", "time": timeout_val, "memory": 0, "output": "", "error": "Time Limit Exceeded", "time_limit": time_limit, "memory_limit": memory_limit_kb if 'memory_limit_kb' in locals() else float('inf')}
    except Exception as e:
        return {"status": "ERR", "time": 0, "memory": 0, "output": "", "error": str(e), "time_limit": 0, "memory_limit": 0}

def main():
    if len(sys.argv) < 3:
        print("Usage: python3 check_solution.py <solution_file.cpp> <test_dir>")
        sys.exit(1)
    
    solution_file = sys.argv[1]
    test_dir = sys.argv[2]
    
    executable = compile_solution(solution_file)
    
    input_files = sorted(glob.glob(os.path.join(test_dir, "*.in")))

    try:
        sorted_input_files = input_files[::]
        sorted_input_files.sort(key = lambda name: int(os.path.basename(name).split('.')[0]))
        input_files = sorted_input_files
    except Exception as e:
        print(f'Failed to sort input files because: {e}')
    
    if not input_files:
        print(f"No .in files found in {test_dir}")
        sys.exit(0)
    
    print(f"{'Test':<10} | {'Status':<6} | {'Time (s)':<10} | {'Limit (s)':<10} | {'Mem (KB)':<10} | {'Limit (KB)':<10}")
    print("-" * 80)
    
    passed = 0
    total = 0
    
    for input_file in input_files:
        base_name = os.path.splitext(os.path.basename(input_file))[0]
        expected_output_file = os.path.join(test_dir, f"{base_name}.out")
        time_limit_file = os.path.join(test_dir, f"{base_name}.time")
        memory_limit_file = os.path.join(test_dir, f"{base_name}.memory")
        
        result = run_test(executable, input_file, expected_output_file, time_limit_file, memory_limit_file)
        
        status_color = ""
        if result["status"] == "OK":
            status_color = "\033[92m" # Green
            passed += 1
        elif result["status"] == "WA":
            status_color = "\033[91m" # Red
        elif result["status"] == "TLE":
            status_color = "\033[93m" # Yellow
        elif result["status"] == "MLE":
            status_color = "\033[93m" # Yellow
        else:
            status_color = "\033[95m" # Magenta
            
        reset_color = "\033[0m"
        
        t_lim_str = f"{result['time_limit']:.2f}" if result['time_limit'] != float('inf') else "-"
        m_lim_str = f"{result['memory_limit']:.0f}" if result['memory_limit'] != float('inf') else "-"

        print(f"{base_name:<10} | {status_color}{result['status']:<6}{reset_color} | {result['time']:.3f}      | {t_lim_str:<10} | {result['memory']:.0f}       | {m_lim_str}")
        
        if result["status"] != "OK" and result["status"] != "???":
             # Optional: print first few lines of diff or error
             pass
        total += 1

    print("-" * 80)
    print(f"Summary: {passed}/{total} passed.")
    
    # Cleanup
    if os.path.exists(executable):
        os.remove(executable)

if __name__ == "__main__":
    main()