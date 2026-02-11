# Lexical-Analyzer-in-C

## 📌 Project Overview
This project is a **Lexical Analyzer (Tokenizer)** implemented in **C**.  
It scans an input source code file and breaks it into meaningful **tokens**, which is the first stage of a compiler.

The analyzer identifies different token types such as:
- Keywords
- Identifiers
- Numbers
- Operators
- Delimiters
- Strings
- Comments  

Whitespace and comments are ignored to generate a clean token stream.

---

## 🎯 Features
- Detects **C language keywords**
- Detects **identifiers** (variable/function names)
- Detects **integer numbers**
- Detects **operators** (`+ - * / = == != <= >= && ||` etc.)
- Detects **delimiters** (`; , ( ) { } [ ]`)
- Detects **string literals**
- Detects **single-line and multi-line comments**
- Ignores unnecessary whitespace
- Produces a token stream output

---

## 🛠️ Tech Stack
- **Language:** C  
- **Platform:** Linux (Ubuntu)  
- **Compiler:** GCC  
- **Tools:** VS Code, Makefile  

---

## 📂 Project Structure
```
Lexical-Analyzer-in-C/
│── main.c
│── lexical.c
│── lexical.h
│── Makefile
│── README.md
```

---

## ⚙️ Compilation & Execution

### ✅ Compile
```bash
make
```

### ✅ Run
```bash
./a.out <input_file.c>
```

Example:
```bash
./a.out sample.c
```

---

## 📌 Output
The program prints tokens in a structured format, such as:
- Token Type
- Token Value

Example token stream:
```
KEYWORD      int
IDENTIFIER   main
DELIMITER    (
DELIMITER    )
DELIMITER    {
...
```

---

## 🚀 Future Enhancements
- Support for floating point numbers
- Support for preprocessor directives (`#include`, `#define`)
- Store tokens into output file instead of printing only
- Improve error handling and reporting

---

## 👤 Author
**Nakul Anil Vadar**  
📧 Email: nakulvadar@gmail.com  
🔗 GitHub: https://github.com/Nakulvadar  
