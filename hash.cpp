#include "hash.h"


hashTable::hashTable(int size):capacity(getPrime(size)),filled(0), data(capacity){}

//public member functions
int hashTable::insert(const std::string &key, void *pv) {
    unsigned int hash_value = hash(key);
    unsigned int position = findPos(key);

    if (filled>=capacity*0.6) {// always see if rehash needs to be done first, so don't have to deal with overfilling problem later
        bool rehashed = rehash();

        if (!rehashed){
            return 2;
        }
    }

    while (position == -1){ //not in ht
        if (!data[hash_value].isOccupied){
            data[hash_value].key = key;
            data[hash_value].isOccupied = true;
            data[hash_value].pv = pv;
            filled++;

            return 0;
        }
        hash_value++;
        hash_value %= capacity;
    }

    return 1; //duplicate found
}

bool hashTable::contains(const std::string &key) { //looking for duplicates
    if (findPos(key) == -1 ) { //key dne in ht
        return false;
    }
    else { //duplicate
        return true;
    }
}

void *hashTable::getPointer(const std::string &key, bool *b){

    unsigned int position = findPos(key);

    if (position == -1){ // key dne
        *b = false;
        return nullptr;
    }
    else{ // key exists
        *b = true;
        return data[position].pv;
    }
}

int hashTable::setPointer(const std::string &key, void *pv){
    unsigned int position = findPos(key);

    if (position == -1){ //key dne in ht
        return 1;
    }
    else{ //key exists in ht
        data[position].pv=pv;
        return 0;
    }
}

bool hashTable::remove(const std::string &key){ //lazy deletion

    unsigned int position = findPos(key);

    if (position == -1){ //key dne
        return false;
    }
    else{ //key exists in ht
        data[position].isDeleted = true;
        return true;
    }
}

//private member functions
int hashTable::hash(const std::string &key) {
    unsigned int hash_value = 0;

    for (char ch : key){
        hash_value = 37 * hash_value + ch; //Ref: Horner's rule from figure 5.4 of Weiss
    }
    return hash_value % capacity;
}

int hashTable::findPos(const std::string &key){ //sees if value exists at index; returns -1 if dne, position if does exist
    unsigned int hash_value = hash(key);

    while (true){ //go until duplicate or empty cell

        if(!data[hash_value].isOccupied){ //index not filled
            return -1;
        }
        else if (data[hash_value].key == key && !data[hash_value].isDeleted){ //duplicate that hasn't been lazily deleted
            return hash_value;
        }

        hash_value++;
        hash_value %= capacity; // prevents going over capacity limit
    }
}

bool hashTable::rehash(){
    unsigned int new_size_min = filled*3; //filled reduced from 60% to 33%
    unsigned int new_prime = getPrime(new_size_min);

    std::vector<hashItem> copy = data; //copy old ht

    data.clear(); //clear old data
    filled=0; //update filled & capacity
    capacity=new_prime;
    data.resize(capacity);

    for (hashItem item: copy){
        if (item.isOccupied && !item.isDeleted){ //if old data contains value, reinsert into new
            insert(item.key, item.pv); //reinsert both key & void ptr
        }
    }

    if (filled < (capacity * 0.6)){ //rehash success

        return true;
    }
    else
    {
        return false;
    }
}


unsigned int hashTable::getPrime(int size){


    unsigned int prime_sequence[] = {1949, 7877, 11551, 52639, 220579, 500069, 700319, 800647, 1000099, 3000073, 6000641, 9001481};

    for (int i=0; i<sizeof(prime_sequence); i++) {
        while (size < prime_sequence[i]) {
            return prime_sequence[i];
        }
    }
    return 0; //all available hash values too small

};
