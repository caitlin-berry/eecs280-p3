# Project 3 - Euchre Game Simulation

## Overview
This project implements a full simulation of the card game Euchre in C++. The program models the rules and gameplay mechanics of Euchre, including dealing, making trump, trick-taking, and scoring.

The project emphasizes object-oriented programming concepts such as abstraction, inheritance, and polymorphism by implementing different player types and game components.

## Features
- Simulates a full Euchre game with four players
- Supports both AI ("Simple") and human-controlled players
- Implements all core game phases:
  - dealing cards
  - making trump (two rounds)
  - trick-taking
  - scoring and win conditions
- Accurately models Euchre-specific rules such as:
  - right and left bowers
  - trump suit hierarchy
  - following suit and trick resolution
- Supports configurable game settings via command line arguments

## How to Run

Compile the program:
```bash
g++ euchre.cpp Card.cpp Pack.cpp Player.cpp -o euchre
```
Run the program:
```bash
./euchre pack.in noshuffle 3 Alice Simple Bob Simple Carol Simple Dave Simple
```
Arguments:
- `pack.in`: input file containing the deck
- `shuffle` or `noshuffle`: whether to shuffle the deck
- number of points to win the game
- player names and types (`Simple` or `Human`)

## Concepts Used
- Object-oriented programming (classes, inheritance, polymorphism)
- Abstract data types (Card, Pack, Player)
- Dynamic memory management
- Vectors and data structures
- Operator overloading
- File input/output
- Game simulation and state management
- Unit testing

## Program Structure
- `Card.cpp` / `Card.hpp`: Represents a playing card and comparison logic
- `Pack.cpp` / `Pack.hpp`: Represents and manages a deck of cards
- `Player.cpp` / `Player.hpp`: Abstract player class with derived Simple and Human players
- `euchre.cpp`: Main game driver and game logic
- `Card_tests.cpp`: Unit tests for Card
- `Pack_tests.cpp`: Unit tests for Pack
- `Player_tests.cpp`: Unit tests for Player behavior
- `pack.in`: Input file representing a deck of cards

## Example Features
- AI player follows a defined strategy for making trump and playing cards
- Human player interacts via console input
- Game output clearly displays each phase:
  - dealing
  - player decisions
  - trick outcomes
  - score updates
- Supports multiple rounds until a team reaches the target score

## Example Output
The program prints a full game simulation, including player actions and results. For example:

Hand 0  
Alice deals  
Jack of Diamonds turned up  

Bob passes  
Carol passes  
Dave passes  
Alice orders up Diamonds  

... (gameplay continues)

Alice and Carol win the hand  
Alice and Carol have 2 points  
