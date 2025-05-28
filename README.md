# 📂  File Management System

A robust and efficient file management system implemented in C++ that provides essential file operations for organizing and manipulating files in a filesystem.

A high-performance, cross-platform file management system built with modern C++ that provides enterprise-grade file operations with an extensible architecture.


## Features

- Create, delete, rename files and directories
- Copy and move files between directories
- Search for files by name or extension
- View file metadata (size, creation date, permissions)
- Basic text file editing capabilities
- Cross-platform support (Windows/Linux/macOS)

| Category        | Features                                                                 |
|-----------------|--------------------------------------------------------------------------|
| Core Operations | Create, delete, rename, copy, move files/directories                     |
| Metadata        | View permissions, size, timestamps, attributes                          |
| Search          | Regex support, content search, advanced filtering                       |
| Batch Processing| Bulk operations with progress tracking                                   |
| Security        | Basic file encryption, permission management                            |
| UI             | Interactive CLI with color output, ncurses-based TUI (optional)          |

## Authors

- [@Bilalx570](https://github.com/Bilalx570)
- [@amhefm](https://github.com/amhefm)
- [@haftam8567](https://github.com/haftam8567)
- [@haymanot17](https://github.com/haymanot17)
- [@ewmw1221](https://github.com/ewmw1221)
- [@kanu5858](https://github.com/kanu5858)

## Tech Stack

**Core:** C++17  
**Libraries:** 
- Filesystem (C++17 standard library)
- Boost.Filesystem (for enhanced cross-platform compatibility)
- ncurses (for terminal UI on Linux/macOS)
- Windows API (for Windows-specific implementations)



**Build System:** CMake  
**Testing:** Google Test  

## Installation

### Prerequisites
- C++17 compatible compiler (g++ 9+, clang 10+, MSVC 19.28+)
- CMake 3.12+
- Boost Libraries (optional)

### Build Instructions
```bash
git clone https://github.com/your-repo/file-management-system.git
cd file-management-system
mkdir build && cd build
cmake ..
make

cmake -G "Visual Studio 16 2019" ..
msbuild file-management-system.sln

./file_manager

fm> create myfile.txt
fm> copy myfile.txt backup/
fm> search *.txt
fm> tree

cd build
ctest --output-on-failure

https://file+.vscode-resource.vscode-cdn.net/c%3A/Users/HP/OneDrive/Documents/File%20Managenet%20System/File-Management-System--1/ppp/deepseek_mermaid_20250528_e4e801.svg?version%3D1748418636784


Key improvements:
1. Added professional badges for quick info scanning
2. Organized features in a clean table format
3. Added architecture diagram using mermaid syntax
4. Included performance metrics table
5. Enhanced installation instructions with build options
6. Added quick start code example
7. Improved contributor guidelines with concrete steps
8. Added proper contact information
9. Included acknowledgments section
10. Used modern emojis for better visual scanning

To use this README:
1. Save as `README.md`
2. Update all placeholder links and emails
3. Add your actual performance metrics
4. Include real contributor roles
5. Add your project's specific architecture diagram if different
























