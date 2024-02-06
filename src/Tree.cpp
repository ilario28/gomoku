/*
** EPITECH PROJECT, 2022
** B-AIA-500-MPL-5-1-gomoku-guillaume.boudaille
** File description:
** Tree
*/

#include "../include/Tree.hpp"

using namespace gmk;

node_t *createNode()
{
    node_t *node = new node_t;

    node->score = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            node->children[i][j] = nullptr;
        }
    }
    return (node);
}

Tree::Tree()
{
    _root = createNode();
}

Tree::~Tree()
{
}

int Tree::evaluate(std::vector<unsigned char> moves,
                    std::array<std::array<bitboard, 2>, 2> boards)
{
    std::vector<unsigned char> tmpmoves;
    bitboard tmpBoard;
    std::array<std::array<bitboard, 2>, 2> boardCopy;
    std::copy(boards.begin(), boards.end(), boardCopy.begin());

    node_t *node = _root;
    for (auto it : moves) {
        if (node->children[it/SIZE][it % SIZE] == nullptr) {
            node->children[it/SIZE][it % SIZE] = createNode();
        }
        node = node->children[it/SIZE][it % SIZE];
    }
        // TODO: place moves on board
    /*
        if(isWin(tmpBoard[WHITE])) {
            node->score = 1;
            return (1);
        }
        if(isWin(tmpBoard[BLACK])) {
            node->score = -1;
            return (-1);
        }
    */
        // TODO: evaluate the score of the node in more depth

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (node->children[i][j] == nullptr && tmpBoard[i][j] == 0) {
                tmpmoves = moves;
                tmpmoves.push_back(i * SIZE + j);
                this->computationQueue.push_back(tmpmoves);
            }
        }
    }
    return 0; // node score
}