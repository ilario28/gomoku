#include "../include/Gomoku.hpp"

using namespace gmk;

Gomoku::Gomoku() : _is_running(true), _is_boarding(false)
{
    _cmds["START"] = cmd{1, &Gomoku::runStart};
    _cmds["END"] = cmd{0, &Gomoku::runEnd};
    _cmds["TURN"] = cmd{1, &Gomoku::runTurn};
    _cmds["BEGIN"] = cmd{0, &Gomoku::runBegin};
    _cmds["BOARD"] = cmd{0, &Gomoku::runBoard};
    _cmds["DONE"] = cmd{0, &Gomoku::runDone};
    _cmds["INFO"] = cmd{2, &Gomoku::runInfo};
    _cmds["ABOUT"] = cmd{0, &Gomoku::runAbout};
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
           logboard[i][j] = '_';
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            _emptyboards[0][i][j] = 1;
            _emptyboards[1][i][j] = 1;
        }
    }
}

Gomoku::~Gomoku()
{}

std::vector<std::string> Gomoku::splitCommand(std::string s, std::string delimiter)
{
    size_t pos_start = 0;
    size_t pos_end;
    size_t delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return (res);
}

void Gomoku::run()
{
    std::string cmd;
    std::vector<std::string> args;
    size_t nbArgs = 0;

    while (this->isRunning()) {
        if (_is_boarding) {
            std::cin >> cmd;
            if (cmd != "DONE") {
                args.push_back(cmd);
                this->runBoard(args);
            } else
                this->runDone(args);
        } else {
            if (cmd.empty()) {
                std::cin >> cmd;
                nbArgs = this->getCmdNbArgs(cmd);
            }

            if (nbArgs > 0) {
                for (size_t i = 0; i < nbArgs; i++) {
                    std::string tmp;
                    std::cin >> tmp;
                    args.push_back(tmp);
                }
            }

            this->runCmd(cmd, args);
        }

        cmd.clear();
        args.clear();
        nbArgs = 0;
    }
}

bool Gomoku::isRunning() const
{
    return (_is_running);
}

void Gomoku::runCmd(const std::string &cmd, std::vector<std::string> args)
{
    if (_cmds.find(cmd) != _cmds.end())
        (this->*(_cmds[cmd].func))(args);
}

void Gomoku::runStart(std::vector<std::string> args)
{
    try {
        std::stringstream ss;
        int value;

        ss << args[0];
        ss >> value;
        if (value != 20)
            std::cout << "ERROR message - unsupported size or other error" << std::endl;
        else
            std::cout << "OK - everything is good" << std::endl;
    } catch (const std::exception &e) {
        std::cout << "ERROR message - unsupported size or other error" << std::endl;
    }
}

void Gomoku::runEnd(std::vector<std::string> args)
{
    (void) args;
    _is_running = false;
}

bool Gomoku::diag(bitboard board, int line)
{
    std::bitset<SIZE> resA = 0b11111111111111111111;
    std::bitset<SIZE> resB = 0b11111111111111111111;
    int offsetA = 0;
    int offsetB = 4;

    for (int i = line; i < line + 5; i++, offsetA++, offsetB--) {
        resA &= (board[i] << offsetA);
        resB &= (board[i] << offsetB);
    }
    return (resA != 0 || resB != 0);
}

bool Gomoku::nlines(bitboard board, int line)
{
    std::bitset<SIZE> res = 0b11111111111111111111;
    for (int i = line; i < line + 5; i++) {
        res &= board[i];
    }
    return (res != 0);
}


bool Gomoku::isWin(std::array<bitboard, 2> boards, int line = 0)
{
    if ((line + 5 )> SIZE)
        return (false);
    return (nlines(boards[DOWN], line) || nlines(boards[RIGHT], line) || diag(boards[DOWN], line) || isWin(boards, line + 1));
}

bool Gomoku::opennlines(bitboard board, int line, bool direction)
{
    bool res;
    std::bitset<SIZE> preli = 0b11111111111111111111;

    for (int i = line; i < line + 4; i++) {
        preli &= board[i];
    }
    res = (preli & _emptyboards[direction][line - 1]).any() && (preli & _emptyboards[direction][line + 4]).any();
    return (res != 0);
}

bool Gomoku::opendiag(bitboard board, int line)
{
    bool resA;
    bool resB;
    std::bitset<SIZE> preliA = 0b11111111111111111111;
    std::bitset<SIZE> preliB = 0b11111111111111111111;
    int offsetA = 0;
    int offsetB = 3;

    for (int i = line; i < line + 4; i++, offsetA++, offsetB--) {
        preliA &= (board[i] << offsetA);
        preliB &= (board[i] << offsetB);
    }
    resA = (preliA & (_emptyboards[DOWN][line - 1] >> 1)).any() && (preliA & (_emptyboards[DOWN][line + 4] << 4)).any();
    resB = (preliB & (_emptyboards[DOWN][line - 1] << 4)).any() && (preliB & (_emptyboards[DOWN][line + 4] >> 1)).any();
    return (resA || resB);
}


bool Gomoku::openCheck(std::array<bitboard, 2> boards, int line = 1)
{
    if ((line + 4 ) > SIZE)
        return (false);
    return (opennlines(boards[DOWN], line, DOWN) || opennlines(boards[RIGHT], line, RIGHT) || opendiag(boards[DOWN], line) || openCheck(boards, line + 1));
}

bool Gomoku::canPlay(int row, int col)
{
    if (row < 0 || row >= SIZE)
        return false;
    if (col < 0 || col >= SIZE)
        return false;
    int colind = SIZE - col - 1;
    std::cout << "CAN PLAY(" << row << "," << colind << "): " << (_emptyboards[RIGHT][row][colind] == 1) << std::endl;
    return (_emptyboards[RIGHT][row][colind] == 1);
}

bool Gomoku::findNextPosition(int row, int col)
{
    int colind = SIZE - col - 1;

    if (_boards[WHITE][RIGHT][row][colind] == 1) {
        for (int offsetA = -1; offsetA <= 1; offsetA++) {
            for (int offsetB = -1; offsetB <= 1; offsetB++) {
                if (offsetA == 0 && offsetB == 0)
                    continue;
                if (canPlay(row + offsetA, col + offsetB)) {
                    row += offsetA;
                    col += offsetB;
                    fillBoard(row, col, WHITE);
                    std::cout << row << "," << col << std::endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void Gomoku::move()
{
    int row = 0;
    int col = 0;

    //checks for immediate win
    bool positionFound = false;
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if(!_emptyboards[RIGHT][row][SIZE - col - 1])
                continue;
            fillBoard(row, col, WHITE);
            positionFound = isWin(_boards[WHITE]);
            fillBoard(row, col, WHITE);
            if (positionFound){
                std::cout << row << "," << col << std::endl;
                return (fillBoard(row, col, WHITE));
            }
        }
    }

    //checks for immediate loss
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if(!_emptyboards[RIGHT][row][SIZE - col - 1])
                continue;
            fillBoard(row, col, BLACK);
            positionFound = isWin(_boards[BLACK]);
            fillBoard(row, col, BLACK);
            if (positionFound){
                std::cout << row << "," << col << std::endl;
                return (fillBoard(row, col, WHITE));
            }
        }
    }

    //check for open 3
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if(!_emptyboards[RIGHT][row][SIZE - col - 1])
                continue;
            fillBoard(row, col, BLACK);
            positionFound = openCheck(_boards[BLACK]);
            fillBoard(row, col, BLACK);
            if (positionFound){
                std::cout << row << "," << col << std::endl;
                return (fillBoard(row, col, WHITE));
            }
        }
    }

    //places new token next to another token
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if (findNextPosition(row, col)) {
                return;
            }
        }
    }

    do {
        row = rand() % SIZE;
        col = rand() % SIZE;
    } while (!_emptyboards[RIGHT][row][SIZE - col - 1]);

    fillBoard(row, col, WHITE);
    std::cout << row << "," << col << std::endl;
}

void Gomoku::printBoards()
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            std::cout << logboard[i][j] << " ";
        }
        std::cout << "\t";
        std::cout << _boards[WHITE][RIGHT][i].to_string() << " ";
        std::cout << "\t";
        std::cout << _boards[WHITE][DOWN][i].to_string() << " ";
        std::cout << "\t";
        std::cout << _boards[BLACK][RIGHT][i].to_string() << " ";
        std::cout << "\t";
        std::cout << _boards[BLACK][DOWN][i].to_string() << " ";
        std::cout << "\t";
        std::cout << _emptyboards[RIGHT][i].to_string() << " ";
        
        std::cout << std::endl;
    }
}

void Gomoku::runTurn(std::vector<std::string> args)
{
    std::stringstream ss;
    std::vector<std::string> datas = this->splitCommand(args[0], ",");

    ss << datas[0];
    int x;
    ss >> x;
    ss.clear();
    ss << datas[1];
    int y;
    ss >> y;
    fillBoard(x, y, BLACK);
    move();
    //printBoards();
}

void Gomoku::runBegin(std::vector<std::string> args)
{
    (void) args;
    std::cout << 10 << "," << 10 << std::endl;
    fillBoard(10, 10, WHITE);
    //printBoards();
}

void Gomoku::boardFill(bitboard *board, bool value)
{
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            (*board)[x][y] = value;
        }
    }
}

void Gomoku::runBoard(std::vector<std::string> args)
{
    if (!_is_boarding) {
        _is_boarding = true;
        // RESET BOARDS
        boardFill(&_boards[WHITE][DOWN], 0);
        boardFill(&_boards[WHITE][RIGHT], 0);
        boardFill(&_boards[BLACK][DOWN], 0);
        boardFill(&_boards[BLACK][RIGHT], 0);
        boardFill(&_emptyboards[DOWN], 1);
        boardFill(&_emptyboards[RIGHT], 1);
    } else {
        std::vector<std::string> datas = this->splitCommand(args[0], ",");
        // INSERT DATA -> BOARDS
        fillBoard(stringToInt(datas[0]), stringToInt(datas[1]), (stringToInt(datas[2]) == 1) ? WHITE : BLACK);
    }
}

void Gomoku::runDone(std::vector<std::string> args)
{
    (void) args;
    _is_boarding = false;
    move();
    //printBoards();
}

void Gomoku::runInfo(std::vector<std::string> args)
{
    std::string key = args[0];
    args.erase(args.begin());
    _vars[key] = args; 
}

void Gomoku::runAbout(std::vector<std::string> args)
{
    (void) args;
    std::cout << "name=\"SomeBrain\", version=\"1.0\", author=\"Guillaume/Noe/Benoit\", country=\"France\"" << std::endl;
}

size_t Gomoku::getCmdNbArgs(const std::string &cmd)
{
    if (_cmds.find(cmd) != _cmds.end())
        return (_cmds[cmd].args);
    return (0);
}

void Gomoku::fillBoard(int row, int col, bool color)
{
    _boards[color][RIGHT][row][(SIZE - col - 1)].flip();
    _boards[color][DOWN][col][(SIZE - row - 1)].flip();
    _emptyboards[RIGHT][row][(SIZE - col - 1)].flip();
    _emptyboards[DOWN][col][(SIZE - row - 1)].flip();

    if (_emptyboards[RIGHT][row][SIZE - col - 1] == 0)
        logboard[row][col] = (color == WHITE ? 'O' : 'X');
    else
        logboard[row][col] = '_';
}

int stringToInt(const std::string &str)
{
    std::stringstream ss;
    ss << str;
    int val;
    ss >> val;
    return (val);
}
