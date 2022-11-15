//heap class implementation

#include "heap.h"

heap::heap(int capacity){
    posCur=1;
    this->capacity = capacity;
    hashTable mapping(capacity*2);
    data.resize(capacity+1);
}

int heap::insert(const std::string &id, const int &key, void *pv){
    if (posCur>capacity){ //current open node past capacity of vector
        return 1;
    }
    else if (mapping.contains(id)){
        return 2;
    }
    else{
        //insert into heap
        data[posCur].id = id;
        data[posCur].key = key;
        data[posCur].pData = pv;

        //insert into hashtable
        mapping.insert(id, &data[posCur]);
        percolateUp(posCur);
        posCur++;

        return 0;
    }
}

int heap::setKey(const std::string &id, int key){
    bool b = false;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b)); //pointer to id

    if (b) { //ht contains key
        int prevKey = (*pn).key;
        (*pn).key = key;

        if(prevKey>key){ //new key less
            percolateUp(getPos(pn));
        }
        else if (key>prevKey) { //new key bigger
            percolateDown(getPos(pn));
        }

        return 0;
    }
    else{ //hashtable/heap does not contain key
        return 1;
    }
}

int heap::remove(const std::string &id, int *pkey, void *ppdata){
    bool b = false;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b)); //pn = address of removed node

    if (b){
        int deletedKey = (*pn).key; //keeps track of deleted key value

        if(pkey){
            *pkey = (*pn).key;
        }
        if(ppdata){
            *(static_cast<void **> (ppdata)) = (*pn).pData;
        }

        mapping.remove(id);
        posCur--;
        *pn = data[posCur]; //insert last node into removed node address
        mapping.setPointer(data[posCur].id, pn); //update hashtable


        if(deletedKey>(*pn).key){ //new key less
            percolateUp(getPos(pn));
        }
        else if ((*pn).key>deletedKey) { //new key bigger
            percolateDown(getPos(pn));
        }

        return 0;
    }
    else{ //node dne
        return 1;
    }

}

int heap::deleteMin(std::string *pID, int *pKey, void *ppData){
    if (posCur<=1){ //empty heap
        return 1;
    }
    else{
        if(pID){
            *pID = data[1].id;
        }
        if(pKey){
            *pKey = data[1].key;
        }
        if(ppData){
            *(static_cast<void **> (ppData)) = data[1].pData;
        }

        mapping.remove(data[1].id);
        posCur--;
        data[1] = data[posCur];
        mapping.setPointer(data[posCur].id, &data[1]); //update hashtable
        percolateDown(1);

        return 0;
    }
}

void heap::percolateUp(int pos) { //logic: compare w/ parent & swap if smaller than parent upto root
    node tempNode = data[pos];

    while (pos > 1) { //prevents checking below root
        if (tempNode.key < data[pos/2].key) { //decimal truncates down in integer division
            data[pos] = data[pos/2];
            mapping.setPointer(data[pos].id, &data[pos]);
            pos = pos/2;
        }
        else{
            break;
        }
    }
    data[pos] = tempNode;
    mapping.setPointer(data[pos].id, &data[pos]);
}


void heap::percolateDown(int pos) { //logic: compare new root w/ children (left first) & switch if larger upto leaf node

    node tempNode = data[pos];
    int left;
    int right;
    int size = posCur-1;
    int child;

    while (true){
        left = pos*2;
        right = pos*2 +1;

        if (left>size){ //leaf node
            break;
        }

        if (left == size){ //filters out nodes with only left children
            child = left;
        }
        else if (data[left].key<data[right].key){
            child = left;
        }
        else{
            child = right;
        }

        if (data[child].key>tempNode.key){
            break;
        }
        else{
            data[pos]=data[child];
            mapping.setPointer(data[pos].id, &data[pos]);
            pos = child;
        }
    }

    data[pos] = tempNode;
    mapping.setPointer(data[pos].id, &data[pos]);
}

int heap::getPos(node *pn){
    int pos = pn - &data[0];

    return pos;
}
