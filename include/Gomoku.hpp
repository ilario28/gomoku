#pragma once

#include <map>
#include <memory>
#include <iostream>
#include <sstream>

#include "Tree.hpp"


namespace gmk
{
    class Gomoku;
    /**
     * @brief Command Structure
     * 
     */
    struct cmd {
        /**
         * @brief Amount of command parameters
         * 
         */
        size_t args;

        /**
         * @brief Pointer on the command function
         * 
         */
        void (Gomoku::*func)(std::vector<std::string>);
    };

    /**
     * @brief Gomoku Class - Main class of the game
     * 
     */
    class Gomoku
    {
        public:
            /**
             * @brief Construct a new Gomoku object
             * 
             */
            Gomoku();

            /**
             * @brief Destroy the Gomoku object
             * 
             */
            ~Gomoku();

            /**
             * @brief Function to run the game
             * 
             */
            void run();

            /**
             * @brief Function to play a turn
             */
            void move();

        private:

            /**
             * @brief fills bitboards with given input
             * @param row
             * @param col
             * @param color indicates if it's on the black or white board
             */
            void fillBoard(int row, int col, bool color);

            /**
             * @brief Each players boards broken into 2 sets of 2 bitboards 
             */
            //4 boards, 2 for each player, 2 for each direction, SIZE x SIZE bitset array
            std::array<std::array<bitboard, 2>, 2> _boards;

            /**
             * @brief a bitboard for the empty spaces
             */
            std::array<bitboard, 2> _emptyboards;

            /**
             * @brief check wether board is winning
             * @param boards the boards to check (a given player's RIGHT and DOWN boards)
             * @param line for recursion
            */
            bool isWin(std::array<bitboard, 2> boards, int line);

            /**
             * @brief check for open 4s
             * @param boards the boards to check (a given player's RIGHT and DOWN boards)
             * @param line for recursion
             */
            bool openCheck(std::array<bitboard, 2> boards, int line);

            /**
             * @brief algorithm to check if a given board is winning (vertically or horizontally)
             * @param board the board to check
             * @param line for recursion
            */
            bool opennlines(bitboard board, int line, bool direction);

            /**
             * @brief algorithm to check if a given board is winning (diagonally)
             * @param board the board to check
             * @param line for recursion
            */
            bool opendiag(bitboard board, int line);

            /**
             * @brief algorithm to check if a given board is winning (vertically or horizontally)
             * @param board the board to check
             * @param line for recursion
            */
            bool nlines(bitboard board, int line);

            /**
             * @brief algorithm to check if a given board is winning (diagonally)
             * @param board the board to check
             * @param line for recursion
            */
            bool diag(bitboard board, int line);

            /**
             * @brief prints the boards
            */
           void printBoards();
            
            /**
             * @brief a char board for logging purposes
             */
            char logboard[SIZE][SIZE];

            /**
             * @brief a bool for which player we are
             */
            bool player = WHITE;

            /**
             * @brief Check if Gomoku is running
             * @return true if gomoku is running, else false
             */
            bool isRunning() const;

            /**
             * @brief 
             * 
             */
            bool _is_running;

            /**
             * @brief 
             * 
             */
            bool _is_boarding;

            /**
             * @brief Splits string on delimiter
             * 
             * @return Returns vector of strings
             */
            std::vector<std::string> splitCommand(std::string s, std::string delimiter);

            /**
             * @brief Run protocol command
             * 
             * @param cmd Procotol command
             */
            void runCmd(const std::string &cmd, std::vector<std::string> args);

            /**
             * @brief Run START command
             * 
             * @param args Args of protocol command
             */
            void runStart(std::vector<std::string> args);

            /**
             * @brief Run END command
             * 
             * @param args Args of protocol command
             */
            void runEnd(std::vector<std::string> args);

            void runTurn(std::vector<std::string> args);
            void runBegin(std::vector<std::string> args);
            void runBoard(std::vector<std::string> args);
            void runDone(std::vector<std::string> args);
            void runInfo(std::vector<std::string> args);
            void runAbout(std::vector<std::string> args);

            size_t getCmdNbArgs(const std::string &cmd);

            /**
             * @brief Indicate if a token can be played on given position
             * 
             * @param row Row index
             * @param col Colone index
             * @return true if a token can be played on given 2D position
             */
            bool canPlay(int row, int col);

            bool findNextPosition(int row, int col);

            void boardFill(bitboard *board, bool value);

            /**
             * @brief Maps that links cmd id to function
             * 
             */
            std::map<std::string, cmd> _cmds;
            std::map<std::string, std::vector<std::string>> _vars;
    };
}

int stringToInt(const std::string &str);
