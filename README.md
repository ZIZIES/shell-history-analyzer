# shell-history-analyzer

`shis` shows which commands you use most often. It reads Bash, Zsh, Windows
PowerShell, or PowerShell 7 history and prints a ranked, color-coded bar graph.

## Flags

| Flag | Description |
| --- | --- |
| `-c <n>` | show the top n commands (default: 20) |
| `-f, --full` | show every command, no cap |
| `-i, --history <path>` | analyze a specific history file |
| `-s, --sudo` | only graph commands run with sudo |
| `-h, --help` | show help |

## Usage

```text
shis [options]

  -c <n>                  show the top n commands (default: 20)
  -f, --full              show every command
  -i, --history <path>    analyze a specific history file
  -s, --sudo              only graph commands run with sudo
  -h, --help              show help
```

For example:

```sh
./build/shis -c 10
./build/shis --full
./build/shis --sudo -c 10
```

Flags can be combined and used in any order, e.g.
`./build/shis --history ~/.bash_history -c 5`.

## Colors

The bar for each command is colored like a heatmap, relative to your
most-used command: blue is rare, red is frequent.

Colors are skipped automatically when output isn't a real terminal (for
example, piping into a file or `less`), and a note is printed to stderr when
that happens. You can also disable colors yourself by setting `NO_COLOR=1`.

## Windows

1. Install [MSYS2](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal.
3. Install the compiler and build tools:

   ```sh
   pacman -Syu
   pacman -S --needed make mingw-w64-ucrt-x86_64-gcc
   ```

   If the first command asks you to close the terminal, reopen the UCRT64
   terminal, run `pacman -Syu` again, and then install the packages.

4. Build and run the analyzer:

   ```sh
   cd /c/Users/your-name/path/to/shell-history-analyzer
   make clean
   make
   ./build/shis.exe
   ```

The Windows build automatically checks these PSReadLine history files, in this
order:

```text
%APPDATA%\Microsoft\PowerShell\PSReadLine\ConsoleHost_history.txt
%APPDATA%\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
```

If you use both PowerShell 7 and Windows PowerShell 5.1, select a history file
explicitly:

```powershell
.\build\shis.exe --history (Get-PSReadLineOption).HistorySavePath
```

You can also run the executable directly from PowerShell after building it in
MSYS2:

```powershell
cd C:\path\to\shell-history-analyzer
.\build\shis.exe -c 10
```

## Linux and macOS

Install a C compiler and `make`, then run:

```sh
make clean
make
./build/shis
```

The analyzer uses `$SHELL` to choose `~/.bash_history` or `~/.zsh_history`.
Bash may not write the current session to disk until it exits; run `history -a`
before `shis` if you want to include the current session.

To analyze another file:

```sh
./build/shis --history /path/to/history
```

## What is counted

`shis` counts the first word on each history line. For example, `git status`
and `git log` both count as `git`. On Windows, command names are compared without
regard to letter casing.

`sudo` is stripped from the front of a command before counting, so
`sudo apt update` counts as `apt`, same as a plain `apt` command. Use
`-s`/`--sudo` to flip this around and graph only the commands you've run
with sudo, keyed by what came after it.
