<div align="center">
<h1 align="center">
<img src="https://raw.githubusercontent.com/PKief/vscode-material-icon-theme/ec559a9f6bfd399b82bb44393651661b08aaf7ba/icons/folder-markdown-open.svg" width="100" />
<br>
42-minishell <a href="https://github.com/JaeSeoKim/badge42"><img src="https://badge42.vercel.app/api/v2/cl27cprhd001109mercwbbu5l/project/2483584" alt="jremy's 42 minishell Score" /></a>
</h1>
<h3 align="center">📍 Shell-ebrate your coding with 42-minishell!</h3>
<h3 align="center">⚙️ Developed with the software and tools below:</h3>

<p align="center">
<img src="https://img.shields.io/badge/GNU%20Bash-4EAA25.svg?style=for-the-badge&logo=GNU-Bash&logoColor=white" alt="GNU%20Bash" />
<img src="https://img.shields.io/badge/C-A8B9CC.svg?style=for-the-badge&logo=C&logoColor=black" alt="C" />
<img src="https://img.shields.io/badge/Markdown-000000.svg?style=for-the-badge&logo=Markdown&logoColor=white" alt="Markdown" />
</p>
</div>

## 📍 Overview

<p align="center">
  <img src="./img/minishell.png" width="538">
</p>


The Minishell project at school 42 provides a deep understanding of processes, file descriptors, command management, redirections, signals, and interaction with the environment. It also emphasizes code structure, readability, and adherence to programming standards. In order to enhance command parsing, structure, and execution, we chose to implement an Abstract Syntax Tree (AST). The AST allows for better organization and management of complex command sequences, grouping commands, and maintaining their relationships in a structured manner. By incorporating an AST, we were able to achieve more advanced features and improve the overall command processing in the Minishell project, while ensuring code quality and adherence to programming principles.

---

## 💫 Features

- Prompt: Minishell displays a prompt to wait for user commands.

- Functional History: The shell includes a functional command history that allows recalling previous commands.

- External Command Execution: Minishell is capable of executing external commands using the execve() function. It searches for specified paths in the PATH environment variable and supports both relative and absolute paths.

- Redirection: The shell supports the following redirections:
    - \<: Redirects input from a file.
    - \>: Redirects output to a file.
    - \>>: Redirects output to a file in append mode.
    - |: Pipes the output of one command to the input of another command.

- Signal Handling: Minishell handles signals such as Ctrl-C, Ctrl-D, and Ctrl-\ in a similar manner to bash.

- Built-in Commands: The shell implements the following built-in commands:

    - echo: Displays text on the screen, including the -n option to remove the trailing newline.
    - cd: Changes the current directory using a relative or absolute path.
    - pwd: Displays the current working directory.
    - export: Manages environment variables.
    - unset: Removes environment variables.
    - env: Displays environment variables.
    - exit: Exits the shell.

- Conditional Execution: Minishell supports conditional execution using the && and || operators. Commands can be executed based on the success or failure of previous commands.

- Wildcard Expansion: The shell supports wildcard expansion using the * character. It can match multiple filenames or directories.
---


<img src="https://raw.githubusercontent.com/PKief/vscode-material-icon-theme/ec559a9f6bfd399b82bb44393651661b08aaf7ba/icons/folder-github-open.svg" width="80" />

## 📂 Project Structure


```bash
repo
├── Makefile
├── README.md
├── includes
│   └── minishell.h
├── libft [...]
│
├── srcs
│   ├── bin
│   │   ├── cd
│   │   │   ├── cd.c
│   │   │   ├── cd.txt
│   │   │   ├── cd_utils.c
│   │   │   └── cd_utils2.c
│   │   ├── echo
│   │   │   ├── echo.c
│   │   │   └── echo.txt
│   │   ├── env
│   │   │   ├── env.c
│   │   │   └── env.txt
│   │   ├── exit
│   │   │   ├── exit.c
│   │   │   └── exit.txt
│   │   ├── export
│   │   │   ├── export.c
│   │   │   ├── export.txt
│   │   │   ├── export_printer.c
│   │   │   ├── export_utils.c
│   │   │   ├── export_utils_2.c
│   │   │   └── export_utils_3.c
│   │   ├── pwd
│   │   │   ├── pwd.c
│   │   │   └── pwd.txt
│   │   └── unset
│   │       ├── unset.c
│   │       └── unset.txt
│   ├── env.c
│   ├── exe
│   │   ├── create_cmd.c
│   │   ├── create_cmd_utils.c
│   │   ├── create_redirect.c
│   │   ├── exe.c
│   │   ├── exe_childs.c
│   │   ├── exe_childs_utils.c
│   │   ├── exe_utils.c
│   │   ├── init_seq.c
│   │   ├── launcher_forks.c
│   │   ├── launcher_forks_utils.c
│   │   ├── redirect.c
│   │   └── redirect_utils.c
│   ├── exit.c
│   ├── exit_utils.c
│   ├── expand
│   │   ├── clean_token.c
│   │   ├── expand.c
│   │   ├── expand_utils.c
│   │   ├── expand_utils_2.c
│   │   ├── expand_utils_3.c
│   │   ├── field_spliting.c
│   │   ├── globe.c
│   │   ├── globe_utils_1.c
│   │   ├── globe_utils_2.c
│   │   ├── globe_utils_3.c
│   │   └── quote_remove.c
│   ├── here_doc
│   │   ├── here_doc.c
│   │   ├── here_doc_utils.c
│   │   └── here_doc_utils2.c
│   ├── lexer
│   │   ├── lexer.c
│   │   └── lexer_utils.c
│   ├── minishell.c
│   ├── minishell_utils.c
│   ├── print_prompt.c
│   ├── signal.c
│   ├── split_quote.c
│   ├── syntax
│   │   ├── syntax_checker.c
│   │   └── syntax_utils.c
│   ├── tokenizer
│   │   ├── tokenizer.c
│   │   ├── tokenizer_utils.c
│   │   └── tokenizer_utils2.c
│   └── tree
│       ├── create_tree.c
│       ├── create_tree_utils.c
│       ├── create_tree_utils_2.c
│       ├── exec_tree.c
│       └── exec_tree_utils.c
└── testeurs
    ├── basic_test.sh
    ├── export_random.sh
    ├── random.sh
    └── test.sh

43 directories, 193 files
```

---

<img src="https://raw.githubusercontent.com/PKief/vscode-material-icon-theme/ec559a9f6bfd399b82bb44393651661b08aaf7ba/icons/folder-src-open.svg" width="80" />

---

## 🚀 Getting Started

### ✅ Prerequisites

Before you begin, ensure that you have the following prerequisites installed:
 - Make
 - GNU Readline library

### 🖥 Installation

1. Clone the 42-minishell repository:
```sh
git clone https://github.com/jremy42/42-minishell
```

2. Change to the project directory:
```sh
cd 42-minishell
```

3. 3. Compiling the project:

mandatory :
```sh
make 
```

### 🤖 Using 42-minishell

non interactive mode : 
```sh
./minishell -c [cmd]
```

interactive mode :
```sh
./minishell
```


### 🧪 Running Tests

for mandatory & bonus test
```sh
cp minishell testeurs/.
./testeurs/test.sh
```

for brutal random input : 
```sh
./testeurs/random.sh | ./minishell
```

for brutal export random value :
```sh
./testeurs/export_random.sh | ./minishell
```

---

## 👏 Acquired knowledge

- ✅ Processes: Understanding and manipulating processes using functions like fork(), wait(), waitpid(), execve(), etc.

- ✅ File descriptors: Using file descriptors with functions like open(), read(), write(), close(), etc.

- ✅ Prompt: Displaying a prompt to wait for a new command from the user.

- ✅ History: Implementing functional command history to recall previous commands.

- ✅ Path access: Searching specified paths in the PATH environment variable for command execution.

- ✅ Redirections: Handling input redirection (<), output redirection (> and >>), and piping input/output between commands using pipes (|).

- ✅ Signals: Managing signals such as Ctrl-C, Ctrl-D, and Ctrl-\ similar to Bash.

- ✅ Built-in commands: Implementing built-in commands like echo, cd, pwd, export, unset, env, and exit.

- ✅ Error handling: Utilizing error handling functions like strerror() and perror() to display appropriate error messages.

- ✅ Wildcards: Supporting wildcards (*) for the current directory.

- ✅ Logical operators: Implementing logical operators && and || with parentheses for precedence.

- ✅ Abstract Syntax Tree (AST): Building and traversing an AST to represent and execute parsed commands, enabling more complex command structures and execution flow.

## 😎 Team :

[Fred](https://profile.intra.42.fr/users/fle-blay), [Jonathan](https://profile.intra.42.fr/users/jremy) 

