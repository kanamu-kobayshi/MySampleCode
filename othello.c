#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define SIZE 8
#define EMPTY '.'
#define PLAYER 'P'
#define COMPUTER 'C'
#define POSSIBLE 'x'

void print_board(char board[][SIZE]);
void init_board(char board[][SIZE]);
bool valid_move(int x, int y, char player, char board[][SIZE]);
bool check_flip(int x, int y, int dx, int dy, char player, char board[][SIZE]);
bool can_play(char player, char board[][SIZE]);
void make_move(int x, int y, char player, char board[][SIZE]);
void flip(int x, int y, int dx, int dy, char player, char board[][SIZE]);
void mark_possible_moves(char player, char board[][SIZE]);
bool game_over(char board[][SIZE]);
int count_score(char player, char board[][SIZE]);
void computer_move(char board[][SIZE]);
int evaluate_board(char board[][SIZE], char player);
int minimax(char board[][SIZE], int depth, bool maximizing_player, char player, int alpha, int beta);

int main()
{
    char board[SIZE][SIZE];
    init_board(board);
    mark_possible_moves(PLAYER, board);
    char current_player = PLAYER;

    bool skip_turn = false;
    bool player_skipped = false;
    bool computer_skipped = false;

    while (!game_over(board))
    {
        print_board(board);

        if (can_play(current_player, board))
        {
            if (current_player == PLAYER)
            {
                int x, y;
                printf("Player's turn (P): y x\n");
                printf("Player's turn (P): ");
                scanf("%d %d", &x, &y);

                if (valid_move(x, y, PLAYER, board))
                {
                    make_move(x, y, PLAYER, board);
                }
                else
                {
                    printf("Invalid move. Try again.\n");
                    continue;
                }
            }
            else
            {
                computer_move(board);
            }

            current_player = (current_player == PLAYER) ? COMPUTER : PLAYER;
            player_skipped = false;
            computer_skipped = false;
        }
        else
        {
            printf("%c cannot make a move.\n", current_player);
            if (current_player == PLAYER)
            {
                player_skipped = true;
            }
            else
            {
                computer_skipped = true;
            }
            current_player = (current_player == PLAYER) ? COMPUTER : PLAYER;

            if (player_skipped && computer_skipped)
            {
                break;
            }
        }
    }

    if (!player_skipped && !computer_skipped)
    {
        printf("No more possible moves.\n");
    }

    print_board(board);
    int player_score = count_score(PLAYER, board);
    int computer_score = count_score(COMPUTER, board);
    printf("Final score: P %d - C %d\n", player_score, computer_score);

    if (player_score > computer_score)
    {
        printf("Player wins!\n");
    }
    else if (computer_score > player_score)
    {
        printf("Computer wins!\n");
    }
    else
    {
        printf("It's a draw!\n");
    }

    sleep(5);
    return 0;
}

void print_board(char board[][SIZE])
{
    printf("  ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        if (i == 0)
        {
            printf(" Current player: %c", PLAYER);
        }
        else if (i == 1)
        {
            printf(" Current player: %c", COMPUTER);
        }
        printf("\n");
    }
}

void init_board(char board[][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = EMPTY;
        }
    }

    int mid = SIZE / 2;
    board[mid - 1][mid - 1] = PLAYER;
    board[mid - 1][mid] = COMPUTER;
    board[mid][mid - 1] = COMPUTER;
    board[mid][mid] = PLAYER;
}

bool valid_move(int x, int y, char player, char board[][SIZE])
{
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y] != POSSIBLE)
    {
        return false;
    }

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }
            if (check_flip(x, y, dx, dy, player, board))
            {
                return true;
            }
        }
    }

    return false;
}

bool check_flip(int x, int y, int dx, int dy, char player, char board[][SIZE])
{
    char opponent = (player == PLAYER) ? COMPUTER : PLAYER;
    x += dx;
    y += dy;

    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y] != opponent)
    {
        return false;
    }

    bool found_opponent = false;
    while (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
    {
        if (board[x][y] == player)
        {
            if (found_opponent)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        if (board[x][y] == EMPTY || board[x][y] == POSSIBLE)
        {
            return false;
        }

        if (board[x][y] == opponent)
        {
            found_opponent = true;
        }

        x += dx;
        y += dy;
    }

    return false;
}

bool can_play(char player, char board[][SIZE])
{
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (valid_move(x, y, player, board))
            {
                return true;
            }
        }
    }

    return false;
}

void make_move(int x, int y, char player, char board[][SIZE])
{
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }
            if (check_flip(x, y, dx, dy, player, board))
            {
                flip(x, y, dx, dy, player, board);
            }
        }
    }

    board[x][y] = player;
    mark_possible_moves((player == PLAYER) ? COMPUTER : PLAYER, board);
}

void flip(int x, int y, int dx, int dy, char player, char board[][SIZE])
{
    x += dx;
    y += dy;

    while (board[x][y] != player)
    {
        board[x][y] = player;
        x += dx;
        y += dy;
    }
}

void mark_possible_moves(char player, char board[][SIZE])
{
    // Reset all possible move markers
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (board[x][y] == POSSIBLE)
            {
                board[x][y] = EMPTY;
            }
        }
    }

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (board[x][y] == EMPTY)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        if (dx == 0 && dy == 0)
                        {
                            continue;
                        }
                        if (check_flip(x, y, dx, dy, player, board))
                        {
                            board[x][y] = POSSIBLE;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool game_over(char board[][SIZE])
{
    bool player_can_play = can_play(PLAYER, board);
    bool computer_can_play = can_play(COMPUTER, board);

    return !player_can_play && !computer_can_play;
}

int count_score(char player, char board[][SIZE])
{
    int score = 0;
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (board[x][y] == player)
            {
                score++;
            }
        }
    }
    return score;
}

void computer_move(char board[][SIZE])
{
    int max_eval = -10000;
    int best_x = -1;
    int best_y = -1;
    int depth = 5;

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (valid_move(x, y, COMPUTER, board))
            {
                char temp_board[SIZE][SIZE];
                memcpy(temp_board, board, SIZE * SIZE * sizeof(char));
                make_move(x, y, COMPUTER, temp_board);

                int eval = minimax(temp_board, depth - 1, false, COMPUTER, -10000, 10000);
                if (eval > max_eval)
                {
                    max_eval = eval;
                    best_x = x;
                    best_y = y;
                }
            }
        }
    }

    if (best_x != -1 && best_y != -1)
    {
        make_move(best_x, best_y, COMPUTER, board);
    }
}

int evaluate_board(char board[][SIZE], char player)
{
    int score = 0;
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (board[x][y] == player)
            {
                score += 1;
            }
            else if (board[x][y] == (player == PLAYER ? COMPUTER : PLAYER))
            {
                score -= 1;
            }
        }
    }
    return score;
}

int minimax(char board[][SIZE], int depth, bool maximizing_player, char player, int alpha, int beta)
{
    if (depth == 0 || game_over(board))
    {
        return evaluate_board(board, player);
    }

    if (maximizing_player)
    {
        int max_eval = -10000;

        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                if (valid_move(x, y, player, board))
                {
                    char temp_board[SIZE][SIZE];
                    memcpy(temp_board, board, SIZE * SIZE * sizeof(char));
                    make_move(x, y, player, temp_board);

                    int eval = minimax(temp_board, depth - 1, false, player, alpha, beta);
                    max_eval = (eval > max_eval) ? eval : max_eval;
                    alpha = (alpha > max_eval) ? alpha : max_eval;
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }

        return max_eval;
    }
    else
    {
        int min_eval = 10000;

        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                if (valid_move(x, y, (player == PLAYER ? COMPUTER : PLAYER), board))
                {
                    char temp_board[SIZE][SIZE];
                    memcpy(temp_board, board, SIZE * SIZE * sizeof(char));
                    make_move(x, y, (player == PLAYER ? COMPUTER : PLAYER), temp_board);

                    int eval = minimax(temp_board, depth - 1, true, player, alpha, beta);
                    min_eval = (eval < min_eval) ? eval : min_eval;
                    beta = (beta < min_eval) ? beta : min_eval;
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }

        return min_eval;
    }
}
