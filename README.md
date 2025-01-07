# Quiz Game Project

This is a simple quiz game project written in C. The project was created as part of a C programming course. The game allows users to select a quiz file, answer questions, and receive a score report.

## Features

- Choose from multiple quiz files.
- Answer multiple-choice questions.
- Receive a score report at the end of the quiz.

## Files

- `app.c`: The main source code file containing the implementation of the quiz game.
- `quiz_*.txt`: Quiz files containing questions and answers in a specific format.

## Quiz File Format

Each quiz file should follow this format:

```
Question 1?
A) Option 1
B) Option 2
C) Option 3
D) Option 4
Correct_Option

Question 2?
A) Option 1
B) Option 2
C) Option 3
D) Option 4
Correct_Option
```

## How to Compile and Run

1. Open a terminal and navigate to the project directory.

2. Compile the program using `gcc`:

   ```sh
   gcc app.c -o quiz_game
   ```

3. Run the compiled program:

   ```sh
   ./quiz_game
   ```

4. Follow the on-screen instructions to enter your name, ID, and choose a quiz file.

## Contributors

- Karim
- Ramy
- Youssef
- Noureldine
- Mathew

## License

This project is for educational purposes and does not have a specific license.
