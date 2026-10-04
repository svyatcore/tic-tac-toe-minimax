#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//!! для windows, для других операционых придется ненмого поменять некоторые моменты


//отчистка окна(эстетик стирание)
void clear_screen() {
    system("cls");
}

//инициализация доски(заполнение ячеек пробелами)
void init_board(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

//вывод доски(эстетик вывод в терминал)
void print_board(char board[3][3]) {

    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        if (i < 2) printf("---+---+---\n");
    }
}

//проверка на побeду(W)
char check_winner(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        //првоерка строк
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
            
        //проверка столбцов
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }   
    
    //проверка диоганалей
    if (board[1][1] != ' ' &&
        ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) || 
        (board[0][2] == board[1][1] && board[1][1] == board[2][0])))
        return board[1][1];
    
    return ' '; // если никто не выиграл - вывод пробела
}

//проверка на залоплненность доски(1 - если доска полностью заполенна)
int is_full(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return 0;
        }
    } return 1;
}

// функция которая делает ход(с проверкой на дибила )
int make_move(char board[3][3], int x, int y, char symbol) {
    if (x < 1 || x > 3 || y < 1 || y > 3) return 0;
    else if (board[x - 1][y - 1] != ' ') return 0;
    
    board[x - 1][y - 1] = symbol;
    return 1;
}


// функция minimax(рукурсивная функция, которая ищет лучший ход для игрока X)
int minimax(char board[3][3], int move) {
    // проверка на победу или ничью с присвоением очков
    // так как бот - X, то за вутку которая приносит очко будо получаем очко, а за ветку которая приносит очко протвнику - получам -1 очко
    if (check_winner(board) == 'X') return 1;
    else if (check_winner(board) == 'O') return -1;
    else if (is_full(board)) return 0;

    int best_score = (move == 0) ? -2 : 2; // если ход бота, то ищем максимальное значение, если ход протвиника, то минимальное

    // если ход бота(X)
    if (move == 0) { // перебор всех возомжных ходов с проверкой на пустоту ячейки
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ' ') {
                    continue;
                }

                // делаем ход, рекурсивно вызываем minimax для слудующего хода
                // затем отменяем ход и проверяем получившиеся очки, если они лучше, то они становятся лучшими, как ни странно :)
                make_move(board, i + 1, j + 1, 'X');
                int score = minimax(board, 1);
                board[i][j] = ' ';
                if (score > best_score) {
                    best_score = score;
                }
            }
        }
    } else { // если ход противника(O), все тоже самое что для бота, только ищем теперь минимальное значение(так как противник тоже хочет победить)
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ' ') {
                    continue;
                }
                make_move(board, i + 1, j + 1, 'O');
                int score = minimax(board, 0);
                board[i][j] = ' ';
                if (score < best_score) {
                    best_score = score;
                }
            }
        }
    }
    return best_score; // возвращаем в зад что-то из вот этого множества {-1, 0, 1}
}



//функция, чтобы получить лучший ход
void best_move(char board[3][3]) {
    // инициализация переменных для хранения лучшего очка и координат лучшего хода 
    int best_score = -3;
    int x = 0;
    int y = 0;

    // перебор всех возможных начал для хода бота
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != ' ' ) {
                continue;
            }
            // ставим X в пустую ячейку и вызываем minimax для оценки этого хода, затем отменяем ход и сравниваем с лучшим ходом 
            make_move(board, i + 1, j + 1, 'X');
            int score = minimax(board, 1);
            board[i][j] = ' ';
            if (score > best_score) {
                best_score = score;
                x = i;
                y = j;
            }
        }
    }
    make_move(board, x + 1, y + 1, 'X'); // делаем лучший ход бота
}

//вывод победителя или ничьи
void print_W_or_D(char board[3][3]) {
    char w = check_winner(board);

    // если выиграл бот, то он крут, если вы(невозможно, если не будете мухлевать) то вы крут, а если ничья, то ничья
    if (w != ' ') {
        if (w == 'X') {
            printf("winner: %c you are so dump!!!! \n" , w);
        } else {
            printf("winner: %c you are so good!!!! \n" , w);
        }
        exit(0);

    } else if (is_full(board)) {
        printf("draw \n");
        exit(0);
    }
}

//main функция, для всееего!
int main() {

    char board[3][3];
    // char player = 0; //чет. - игрок номер 1, нечет. - игрок номер 2, это для того чтобы играть без бота 
    int x = 0;
    int y = 0;

    init_board(board);
    print_board(board);

    while (1) {
        char symbol = 'O';
        
        // ввод двух чисел(координат от 1 до 3 (x, y)) и проверка на дебила
        printf("move: %c \n", symbol);
        scanf("%d %d", &x, &y);
        if (!make_move(board, x, y, symbol)) {
            clear_screen();
            printf("you are debil, try again \n");
            print_board(board);
            continue;
        }
 
        clear_screen(); // отчищаем терминал перед выводом
        print_board(board); // выводим эстетик поле в терминал
        print_W_or_D(board); // проверка на победителя или ничьей

        fflush(stdout); // для принудительного сброса буфера
        Sleep(1000); // ждем секунду, нам же пофиг что си перебирает 9! ходов 
        clear_screen(); // отчистка терминала
        best_move(board); // делаем шедевро ход
        print_board(board); // вывод доски
        print_W_or_D(board); // проверка на победителя и ничьей, проиграть можешь только ты!
    }
    return 0;
}