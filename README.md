# CodePort Agent

CodePort Agent is an agentic AI tool that converts Python code into high-performance C++ code.

The project does more than simple code generation. It runs the original Python code, sends it to an OpenAI model to generate equivalent C++ code, writes the generated code to `generated/main.cpp`, compiles it using `g++`, runs the generated executable, and if a compilation or runtime error happens, it sends the error back to the model so it can repair the code automatically.

This project was built as a practical AI Engineering experiment to explore code generation, compiler feedback loops, agentic workflows, and software optimization.

---

## Features

- Convert Python code into C++ using an OpenAI model.
- Run the original Python code and capture its output.
- Save generated C++ code into `generated/main.cpp`.
- Compile generated C++ code using `g++`.
- Run the compiled executable.
- Capture compilation errors and runtime errors.
- Automatically ask the model to fix broken generated code.
- Retry the generate, compile, run, and fix loop for a configurable number of attempts.
- Simple Gradio interface for testing Python-to-C++ conversion.
- Supports model selection between `gpt-5-mini` and `gpt-5`.

---

## How It Works

The main workflow is:

1. The user provides Python code.
2. The original Python code is executed.
3. The Python output is captured.
4. The Python code is sent to an OpenAI model.
5. The model generates C++ code.
6. The generated C++ code is cleaned from Markdown formatting.
7. The C++ code is written to `generated/main.cpp`.
8. The system compiles the C++ code using `g++`.
9. The compiled program is executed.
10. If compilation or runtime fails, the error is sent back to the model.
11. The model repairs the C++ code.
12. The process repeats until success or until the maximum number of attempts is reached.

---

## Agentic Loop

The core idea of this project is the agentic loop:

```text
Generate code
Compile code
Run program
Read errors
Fix code
Retry
```

This makes CodePort Agent closer to a real AI coding agent, because it does not only generate code once. It uses compiler and runtime feedback to improve its own output.

---

## Tech Stack

- Python
- OpenAI API
- Gradio
- python-dotenv
- C++
- g++
- MSYS2 on Windows
- Jupyter Notebook

---

## Project Structure

```text
CodePort-Agent/
│
├── .venv/
├── generated/
│   ├── main.cpp
│   └── main.exe
│
├── samples/
├── .env
├── requirements.txt
├── codeport_agent.ipynb
└── README.md
```

---

## Setup

### 1. Create the project folder

```bash
mkdir CodePort-Agent
cd CodePort-Agent
```

### 2. Create a virtual environment

On Windows PowerShell:

```bash
python -m venv .venv
```

Activate it:

```bash
.\.venv\Scripts\Activate.ps1
```

If activation is blocked, run:

```bash
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

Then activate again:

```bash
.\.venv\Scripts\Activate.ps1
```

---

## Install Dependencies

Install the required packages:

```bash
pip install openai python-dotenv gradio ipykernel
```

Or install them from `requirements.txt`:

```bash
pip install -r requirements.txt
```

---

## requirements.txt

```txt
openai
python-dotenv
gradio
ipykernel
```

---

## Environment Variables

Create a `.env` file in the project root:

```env
OPENAI_API_KEY=your_openai_api_key_here
```

Do not share this file publicly.

Do not upload `.env` to GitHub.

---

## C++ Compiler Setup

This project uses `g++` to compile generated C++ code.

On Windows, install MSYS2 and make sure this path exists:

```text
C:\msys64\ucrt64\bin
```

The notebook adds this path automatically:

```python
os.environ["PATH"] = r"C:\msys64\ucrt64\bin;" + os.environ["PATH"]
```

To test if `g++` is working, run:

```bash
g++ --version
```

---

## Jupyter Kernel Setup

Add the virtual environment as a Jupyter kernel:

```bash
python -m ipykernel install --user --name codeport-agent --display-name "Python (CodePort Agent)"
```

Then open `codeport_agent.ipynb` and select:

```text
Python (CodePort Agent)
```

---

## Main Components

### 1. Python Runner

The project runs the original Python code and captures its output. This output is used as the reference result.

### 2. Code Generator

The Python code is sent to an OpenAI model with instructions to return valid C++ code only.

### 3. Code Cleaner

The model output is cleaned to remove Markdown fences such as:

````text
```cpp
````

This ensures only raw C++ code is written into `main.cpp`.

### 4. C++ File Writer

The generated C++ code is saved to:

```text
generated/main.cpp
```

### 5. Compiler

The generated C++ file is compiled using:

```bash
g++ -std=c++20 -O3 generated/main.cpp -o generated/main.exe
```

### 6. Program Runner

The compiled program is executed from:

```text
generated/main.exe
```

### 7. Auto-Fix Loop

If the generated C++ code fails to compile or run, the error message is sent back to the model. The model then returns a corrected version of the code.

---

## Gradio Interface

The project includes a simple Gradio interface where the user can:

- Paste Python code.
- Choose the model.
- Choose the maximum number of repair attempts.
- View the generated C++ code.
- View the original Python output.
- View the generated C++ output.
- View the agent logs.

Run the Gradio app from the notebook using:

```python
demo.launch(inbrowser=True)
```

---

## Example 1: Factorial

Input Python code:

```python
def factorial(n):
    result = 1
    for i in range(1, n + 1):
        result *= i
    return result

print("Factorial:", factorial(10))
```

Expected output:

```text
Factorial: 3628800
```

---

## Example 2: Maximum Subarray Sum

Input Python code:

```python
def max_subarray_sum(nums):
    best = nums[0]
    current = nums[0]

    for x in nums[1:]:
        current = max(x, current + x)
        best = max(best, current)

    return best

nums = [3, -5, 10, -2, 4, -20, 7, 8]
print("Max subarray sum:", max_subarray_sum(nums))
```

Expected output:

```text
Max subarray sum: 12
```

---

## Supported Models

The current UI supports:

- `gpt-5-mini`
- `gpt-5`

Recommended usage:

- Use `gpt-5-mini` for cheaper and faster experiments.
- Use `gpt-5` for more difficult code generation tasks.

---

## Current Status

MVP completed.

Current version:

```text
Python → C++ → Compile → Run → Auto-fix errors
```

---

## Limitations

This is an MVP version.

Current limitations:

- It converts one Python snippet at a time.
- It currently targets C++ only.
- It does not yet support full multi-file repositories.
- It does not deeply verify semantic equivalence beyond output comparison.
- It uses `exec()` to run Python code.
- It compiles and runs generated C++ code locally.
- It is designed for local experimentation, not public SaaS deployment.

---

## Security Warning

This project executes Python code using `exec()` and also compiles and runs generated C++ code.

Only run code that you trust.

Do not use this project to run untrusted user-submitted code unless you add a secure sandboxing system first.

---

## Future Improvements

- Add Rust support.
- Add Go support.
- Add file upload support.
- Support converting one Python file instead of only pasted code.
- Support multi-file Python projects.
- Add automatic unit test generation.
- Add stronger output comparison.
- Add support for `pytest`.
- Add a LangGraph-based agent workflow.
- Add Docker sandboxing for safer execution.
- Save experiment results into a CSV or database.
- Add model benchmarking.
- Add support for local models through Ollama.
- Add support for OpenRouter or Groq models.

---

## Roadmap

### Version 1

Single Python snippet to C++ conversion with compile, run, and auto-fix loop.

### Version 2

File upload and single-file conversion.

### Version 3

Automatic output comparison and unit test generation.

### Version 4

Multi-file project support.

### Version 5

Agentic repository porter that can read a repo, understand file relationships, convert files one by one, compile the project, run tests, fix errors, and retry.

---

## What I Learned

This project helped me understand:

- How to build an LLM-powered developer tool.
- How to use compiler feedback inside an AI workflow.
- How to design a simple agentic loop.
- How to connect OpenAI models with local tools.
- How to build a Gradio interface.
- How to use `subprocess` for compile and run automation.
- How to think about correctness, runtime errors, and repair attempts.
- How model output can improve when given structured error feedback.

---

## Author

Mohammed Akram

AI Engineering / Agentic AI Portfolio Project
