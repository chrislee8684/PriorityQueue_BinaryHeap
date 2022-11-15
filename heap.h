//Objective: binary heap class as priority queue
//heap class declaration

#ifndef _HEAP_H
#define _HEAP_H

#include <vector>
#include <string>
#include <iostream>
#include "hash.h"

class heap{

public:

    //constructor to initialize heap
    heap(int capacity);

    //Insert string ID, int key, and optional pointer into heap as a node.
    //Key used to compare priority
    //Return: 0 (success), 1 (heap filled to capacity), or 2 (node w/ given id already exists)
    int insert(const std::string &id, const int &key, void *pv = nullptr);

    //Look up string ID & allow user to change key value to provided value
    //Return: 0 (success), or 1 (node w/ given ID dne)
    int setKey(const std::string &id, int key);

    //Delete node with specific ID from heap
    //If pKey supplied, write key of node being deleted to address of key
    //If ppData supplied, write associated void pointer to address
    //Return: 0 (success), or 1 (if node w/ given ID dne)
    int remove(const std::string &id, int *pKey = nullptr, void *ppData = nullptr);

    //delete node w/ smallest key and return data
    //If pId (pointer to ID) supplied, write ID of node being deleted to address of ID
    //If pKey (pointer to key) supplied, write key of node being deleted to adress of key
    //If ppData, write associated void pointer to address
    //Return: 0 (success), or 1 (if heap empty)
    //ex of filling in ppData: *(static_cast<void **> (ppData)) = data[1].pData;
    int deleteMin(std::string *pID = nullptr, int *pKey = nullptr, void *ppData = nullptr);


private:

    //A node in heap consists of:
    //id - unique string ID
    //key - int key to indicate importance (can be negative)
    //pv - pointer related to key
    class node {
    public:
        std::string id{""};
        int key;
        void *pData {nullptr};

        node() = default;
    };

    int capacity; //capacity of heap
    std::vector<node> data; // vector to store entries
    hashTable mapping; //hashtable used to make member functions' average time complexity =  constant
    int posCur; // Current open node.

    void percolateUp(int posCur); //algorithm for insertion (bottom -> top)
    void percolateDown(int posCur); //algorithm for deleteMin (top -> bottom)
    int getPos(node *pn);

};

#endif




















