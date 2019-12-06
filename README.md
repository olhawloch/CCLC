ChessBot allows you to play a game of chess, accomodating for both human and computer players. In particuler, human vs. human, human vs. computer and computer vs. computer are all possible.
Here is how to play.

After running the program:
  "game [WHITE-PLAYER] [BLACK-PLAYER]" starts a new game where [WHITE-PLAYER] & [BLACK-PLAYER]
  can be either human, computer1, computer2, computer3, computer4, where the level of
  difficulty increases from computer1 to computer4
  
  "resign" concedes the game to your opponent. This is the only way, outside of winning or
  drawing the game, to end a game.
  
  A move consists of the command move, followed by the starting and ending coordinates of the 
  piece to be moved. For example: "move e2 e4". Castling would specified by the two- square
  move for the king: move e1 g1 or move e1 c1 for white. Pawn promotion would additionally
  specify the piece type to which the pawn is promoted: "move e7 e8 Q". In the case of a
  computer player, the command move (without arguments) makes the computer player make a move.
  
  "setup" enters setup mode, within which you can set up your own initial board configurations.
  This can only be done when a game is not currently running. Within setup mode, the following
  language is used:
    "+ K e1" places the piece K (i.e., white king in this case) on the square e1. If a piece is
    already on that square, it is replaced. The board should be redisplayed.
    "- e1" removes the piece from the square e1 and then redisplays the board. If there is no
    piece at that square, take no action.
    "= colour" makes it colour’s turn to go next.
    "done" leaves setup mode.
   Upon leaving setup mode, ensure the board contains exactly one
   white king and exactly one black king; that no pawns are on the first or last row of the
   board; and that neither king is in check.
    
    To undo a move, simply type "undo" into the command line, and the last move will be undone,
    allowing you to remake a move if you incorrectly moved a piece.
    
    A win awards one point to the winner and zero points to the loser. A draw awards half a
    point to each team. When the program ends (ctrl-D is pressed), the final score is printed.

We hope you like the game! :<
