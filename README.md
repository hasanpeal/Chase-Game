
# Networked Chess Application

## Overview
This repository contains the implementation of a **Networked Chess Application** developed in C. It was built as part of a homework assignment for the **CSE 220: Systems Fundamentals I** course at Stony Brook University. The project combines concepts of **systems programming** with real-world applications, enabling two players to engage in a simplified chess game over a network.

Key aspects of the project include **socket programming**, **state synchronization**, and **chess logic implementation**. Although not all chess rules are implemented (e.g., en passant, castling, check, and checkmate are excluded), the application serves as an excellent demonstration of core programming and networking principles.

---

## Key Features
- **Chess Game Initialization**: Sets up a valid chessboard state, ready for gameplay.
- **Move Validation**: Ensures all moves follow standard chess rules for pawns, rooks, knights, bishops, queens, and kings.
- **Client-Server Communication**: Uses the **C sockets API** for seamless message exchange.
- **FEN String Parsing & Generation**: Implements Forsyth-Edwards Notation (FEN) for board state representation.
- **Game Save & Load**: Allows players to save and reload games using usernames.
- **Command-based Input**: Supports commands like `/move`, `/save`, `/load`, and `/forfeit`.
- **Network Programming**: Establishes communication between client and server applications.

---

## My Experience
This project was a part of my coursework for **CSE 220: Systems Fundamentals I** at Stony Brook University. Through this assignment, I gained hands-on experience with:
- **C programming fundamentals**.
- **Socket programming** for client-server communication.
- **Chess game logic and state management**.
- **Parsing structured data formats like FEN strings**.
- **Debugging and testing networked applications**.

It was an enriching learning experience that helped me improve my problem-solving skills and understand how real-world applications are structured.

---

## Getting Started

### Prerequisites
- **C Compiler**: GCC or Clang
- **CMake**: For building the project
- **Linux/MacOS/WSL**: Recommended for optimal performance

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/networked-chess.git
   cd networked-chess
   ```
2. Configure the build:
   ```bash
   cmake -S . -B build
   ```
3. Compile the project:
   ```bash
   cmake --build build
   ```
4. Run the tests:
   ```bash
   ./build/run_tests
   ```

### Running the Application
1. Start the server:
   ```bash
   ./build/hw4_server
   ```
2. Connect the client:
   ```bash
   ./build/hw4_client
   ```

---

## Commands
- **/move `<move_string>`**: Make a move (e.g., `/move e2e4`).
- **/save `<username>`**: Save the current game state for a username.
- **/load `<username> <save_number>`**: Load a previously saved game.
- **/forfeit**: Forfeit the game.
- **/import `<FEN_string>`**: Import a game state using FEN notation.
- **/chessboard**: Display the current state of the chessboard.

---

## Learning Outcomes
- **Socket Programming**: Designed a client-server model to enable real-time communication.
- **Game Logic Implementation**: Built a robust framework for validating chess moves.
- **FEN String Manipulation**: Parsed and generated board states in a standardized format.
- **Systems Programming in C**: Applied theoretical knowledge to a practical problem.
- **File I/O**: Enabled persistent game storage using files.

---

## Topics
- `C Programming`
- `Socket Programming`
- `Networked Applications`
- `Chess Game Development`
- `FEN Notation`
- `State Synchronization`
- `Systems Fundamentals`
- `Game Development with C`

---

## Acknowledgments
- This project was completed as part of **CSE 220: Systems Fundamentals I** at Stony Brook University.
- Starter code and assignment guidelines were provided by the course instructor.

---

## License
This project is licensed under the [MIT License](LICENSE).

---

### Keywords
**C programming, networked chess application, socket programming in C, chess FEN string, systems programming project, chess game development, client-server architecture, chess logic implementation in C, network applications, game state synchronization**.
