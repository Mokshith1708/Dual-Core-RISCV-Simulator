import subprocess
import os
from flask import Flask, render_template, request,send_from_directory

app = Flask(__name__)

@app.route('/')
def index():
    # Read the content of core1.s and core2.s from the 'data_files/input/' directory
    core1_path = os.path.join(app.root_path, 'data_files', 'input', 'core1.s')
    with open(core1_path, 'r') as f:
        core1_text = f.read()
    
    core2_path = os.path.join(app.root_path, 'data_files', 'input', 'core2.s')
    with open(core2_path, 'r') as f:
        core2_text = f.read()
    
    return render_template('pages/main_page.html', core1_text=core1_text, core2_text=core2_text)

@app.route('/core1.s')
def get_core1_file():
    return send_from_directory(os.path.join(app.root_path, 'data_files', 'input'), 'core1.s')

@app.route('/core2.s')
def get_core2_file():
    return send_from_directory(os.path.join(app.root_path, 'data_files', 'input'), 'core2.s')

@app.route('/update_core1', methods=['POST'])
def update_core1():
    core1_text = request.form['core1_text']
    with open('data_files/input/core1.s', 'w') as f:
        f.write(core1_text)
    return 'Data for Core 1 updated successfully.'

@app.route('/update_core2', methods=['POST'])
def update_core2():
    core2_text = request.form['core2_text']
    with open('data_files/input/core2.s', 'w') as f:
        f.write(core2_text)
    return 'Data for Core 2 updated successfully.'

@app.route('/compile_simulator', methods=['POST'])
def compile_simulator():
    forwarding = request.form.get('forwarding', type=int)
    policy = request.form.get('policy', type=int)
    add_latency = request.form.get('add_latency', type=int)
    sub_latency = request.form.get('sub_latency', type=int)
    mul_latency = request.form.get('mul_latency', type=int)
    addi_latency = request.form.get('addi_latency', type=int)
    muli_latency = request.form.get('muli_latency', type=int)
    os.chdir('Pipelined_withCache')
    
    compile_result = subprocess.run(['g++', 'simulator.cpp', '-o', 'a'], capture_output=True)

    if compile_result.returncode == 0:

        execute_result = subprocess.run(['./a', str(forwarding), str(policy), str(add_latency), str(sub_latency), str(mul_latency), str(addi_latency), str(muli_latency)], capture_output=True)
        if execute_result.returncode == 0:
            os.chdir('..')
            return 'Simulator compiled and executed successfully.'
        else:
            return f'Error executing simulator: {execute_result.stderr.decode()}'
    else:
        os.chdir('..')
        return f'Error compiling simulator: {compile_result.stderr.decode()}'
def read_register_values(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        register_values = {}
        for line in lines[2:34]:
            key, value = line.strip().split(':')
            register_values[key.strip()] = value.strip()
        return register_values

@app.route('/register_values')
def register_values():

    terminal1_content = read_register_values('data_files/output/terminal1.txt')
    terminal2_content = read_register_values('data_files/output/terminal2.txt')

    return render_template('pages/register_values.html', terminal1_content=terminal1_content, terminal2_content=terminal2_content)

def read_memory_values(file_path):
    with open(file_path, 'r') as file:
        lines_mem = file.readlines()
        memory_values = {}
        start_index = None
        
        for i, line_mem in enumerate(lines_mem):
            if 'Address0' in line_mem:
                start_index = i
                break
        
        if start_index is not None:
            for line_mem in lines_mem[start_index : start_index + 512]:
                key_mem, value_mem = line_mem.strip().split(':')
                memory_values[key_mem.strip()] = value_mem.strip()
        return memory_values

@app.route('/memory_values')
def memory_values():
    terminal1_content_mem = read_memory_values('data_files/output/terminal1.txt')
    terminal2_content_mem = read_memory_values('data_files/output/terminal2.txt')

    return render_template('pages/memory_values.html', terminal1_content_mem=terminal1_content_mem, terminal2_content_mem=terminal2_content_mem)
def read_outputs(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        output_data = []
        for line in lines[:-1]:  # Exclude the last line
            if line.strip():
                line = line[4:]  # Ignore the first 4 characters
                line_parts = line.strip().split(':')
                if len(line_parts) == 2:
                    key, value = line_parts
                    output_data.append((key.strip(), value.strip()))
        return output_data

@app.route('/outputs')
def outputs():
    output_data = read_outputs('data_files/output/OUTPUTS.txt')
    return render_template('pages/outputs.html', output_data=output_data)

if __name__ == '__main__':
    app.run(debug=True)