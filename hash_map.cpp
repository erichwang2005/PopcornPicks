
#include <iostream>
#include "hash_map.h"
using namespace std;
//Constructors for keyValuePairs, no default constructor for movie. So initialization list must be used
popPickTable::keyValuePair::keyValuePair() : key(""), value("") {}

popPickTable::keyValuePair::keyValuePair(string k, string v) : key(k), value(v) {}

popPickTable::popPickTable(long size)
{
    table.resize(size);
    occupied.resize(size, false);
    numElements = 0;
}

//Inspired by DJB Hash Function
int popPickTable::hashFunction(const string& key) const {
    unsigned long hash = 5381;
    for (char ch : key) {
        hash = ((hash << 5) + hash) + ch;
    }
    return hash % table.size();
}

void popPickTable::rehash()
{
    int newTableSize = table.size() * 2;
    vector<keyValuePair> oldTable = table;
    vector<bool> oldOccupied = occupied;

    table.clear();
    occupied.clear();
    table.resize(newTableSize);
    occupied.resize(newTableSize, false);
    numElements = 0;

    for (int i = 0; i < oldTable.size(); i++)
    {
        if (i < oldOccupied.size() && oldOccupied[i])  // Ensure i is within bounds
        {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }
}

void popPickTable::insert(const string& key, string value)
{
    if (static_cast<double>(numElements) / table.size() >= loadThreshold)
    {
        rehash();
    }

    int index = hashFunction(key);
    int startIndex = index;

    do {
        if (!occupied[index]) {
            // Insert the new key-value pair
            table[index] = keyValuePair(key, value);
            occupied[index] = true;
            numElements++;
            return;
        }
        else if (table[index].key == key) {
            // Update existing key's value
            table[index].value = value;
            return;
        }
        index = (index + 1) % table.size();
    } while (index != startIndex);

}

string popPickTable::searchTitle(const string& key)
{
    int index = hashFunction(key);
    int originalIndex = index;

    while (occupied[index])
    {
        if (table[index].key == key)
        {
            return table[index].key;
        }
        index = (index + 1) % table.size();
    }

    return "Not Found"; // not found
}

string popPickTable::searchGenres(const string& key)
{
    int index = hashFunction(key);
    int originalIndex = index;

    while (occupied[index])
    {
        if (table[index].key == key)
        {
            return table[index].value;
        }
        index = (index + 1) % table.size();
    }

    return {};
}

int popPickTable::size()
{
    return table.size();
}