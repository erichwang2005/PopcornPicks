#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "movie.cpp"


using namespace std;

class popPickTable
{
    private:
        struct keyValuePair
        {
            string key;
            string value;
            keyValuePair();
            keyValuePair(string k, string v);
        };

        
        long numElements;
        const double loadThreshold = 0.6;

        void rehash();
        int hashFunction(const string& key) const;

    public:
        popPickTable(long size = 10);
        void insert(const string& key, string value);
        string searchTitle(const string& key);
        string searchGenres(const string& key);
        void printTable() const;
        int size();
        vector<bool> occupied;
        vector<keyValuePair> table;



        

        
};
