/*
** EPITECH PROJECT, 2022
** B-AIA-500-MPL-5-1-gomoku-guillaume.boudaille
** File description:
** Tree
*/

#pragma once

#include <bitset>
#include <array>
#include <string>
#include <list>
#include <vector>
#include <algorithm>

#define SIZE 20

#define WHITE false // == 0
#define BLACK true // == 1

#define RIGHT 0
#define DOWN 1 

typedef std::array<std::bitset<SIZE>, SIZE> bitboard;

namespace gmk {

    typedef struct node_s {
        int score;
        struct node_s *children[SIZE][SIZE];
    } node_t;

    /**
     * @brief function to create a node
     * @return node_t* 
     */
    node_t *createNode();

    /**
     * @brief class to handle possibility tree
     * 
     */
    class Tree {
        public:
            Tree();
            ~Tree();

            std::list<std::vector<unsigned char>> computationQueue;

            /**
             * @brief function evaluate the score of a node
             * 
             */
            int evaluate(std::vector<unsigned char> moves,
                            std::array<std::array<bitboard, 2>, 2> boards);
        private:
            node_t *_root;
    };
}