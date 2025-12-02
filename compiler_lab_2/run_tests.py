import os
import subprocess
import sys

def normalize_text(text):
    # Normalize line endings and strip trailing whitespace
    return text.replace('\r\n', '\n').strip()

def run_tests():
    # Get the current working directory (workspace root)
    base_dir = os.getcwd()
    
    # Define paths
    parser_dir = os.path.join(base_dir, 'incompleted')
    parser_exe = os.path.join(parser_dir, 'parser.exe')
    test_dir = os.path.join(base_dir, 'test')
    output_dir = os.path.join(base_dir, 'output')

    # Ensure output directory exists
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    print(f"Starting tests...")
    print(f"Parser: {parser_exe}")
    print(f"Test Dir: {test_dir}")
    print(f"Output Dir: {output_dir}")
    print("-" * 50)

    for i in range(1, 8):
        input_filename = f'example{i}.kpl'
        result_filename = f'result{i}.txt'
        output_filename = f'example{i}.kpl' # User requested output filename format

        input_path = os.path.join(test_dir, input_filename)
        expected_path = os.path.join(test_dir, result_filename)
        output_path = os.path.join(output_dir, output_filename)

        # Check if input file exists
        if not os.path.exists(input_path):
            print(f"Test {i}: SKIPPED (Input file {input_filename} not found)")
            continue

        print(f"Running Test {i} ({input_filename})...")
        
        try:
            # Run parser.exe
            # We run it from the 'incompleted' directory as in the user's example
            # Command: parser.exe ..\test\exampleX.kpl
            # We use relative path for input file relative to parser_dir
            
            rel_input_path = os.path.relpath(input_path, parser_dir)
            
            with open(output_path, 'w') as outfile:
                # Run the command
                # cmd /c is not needed if we call the exe directly, but let's be safe with subprocess
                cmd = [parser_exe, rel_input_path]
                
                # Capture stdout to outfile
                process = subprocess.run(
                    cmd, 
                    stdout=outfile, 
                    stderr=subprocess.PIPE, 
                    cwd=parser_dir, 
                    text=True
                )
                
                if process.returncode != 0:
                    print(f"  Runtime Error: {process.stderr}")

        except Exception as e:
            print(f"  Execution Exception: {e}")
            continue

        # Compare results
        if not os.path.exists(expected_file := expected_path):
            print(f"  WARNING: Expected result file {result_filename} not found. Cannot verify.")
            continue

        try:
            with open(output_path, 'r') as f_out:
                content_out = normalize_text(f_out.read())
            
            with open(expected_path, 'r') as f_exp:
                content_exp = normalize_text(f_exp.read())

            if content_out == content_exp:
                print(f"  Result: PASS")
            else:
                print(f"  Result: FAIL")
                print(f"  (Check {output_path} vs {expected_path})")
                
        except Exception as e:
            print(f"  Comparison Exception: {e}")
        
        print("-" * 20)

if __name__ == "__main__":
    run_tests()
