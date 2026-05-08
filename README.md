# C-Chess-Engine
A high-performance chess engine implemented in C++ utilizing the Minimax algorithm with Alpha-Beta pruning.

## Features
*   **Minimax Algorithm:** Simulates future board states to a depth of 3+ ply.
*   **Alpha-Beta Pruning:** Eliminates sub-optimal branches to optimize search time.
*   **Dynamic Evaluation:** Uses material weights and Piece-Square Tables (PSTs) for strategic positioning.
*   **Endgame Logic:** Adapts King behavior based on material remaining on the board.

  ## Technical Implementation
### Search Logic
The engine uses a recursive Minimax search. To handle the exponential growth of the move tree, **Alpha-Beta pruning** is used to "prune" branches that are mathematically guaranteed to be worse than previously analyzed moves.
### Evaluation Function
The board is evaluated based on:
$$Score = \sum Material + \sum PositionBonus$$
Where:
*   **Material:** assigned weights (e.g., Queen = 900, Pawn = 100).
*   **Position:** Piece-Square Tables to evaluate position of the pieces on the board
