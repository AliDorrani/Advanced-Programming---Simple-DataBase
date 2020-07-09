#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Table
{
    string name;
    vector < vector<string> > data;
};

#define SPECEFIED_COLOUMN_DATA 0
#define COLOUMNS_DATA_MATCH 1

struct Condition
{
    int type;
    // if type = SPECEFIED_COLOUMN_DATA a = coloumn name , b = coloumn data
    // iif type == COLOUMNS_DATA_MATCH a = coloumn1 , b = coloumn2
    string a,b;
};



vector < string > split (string str , string delimiters );
Table create_table(string table_name, vector<string> headers);
bool insert(Table &table, vector<string> row_info);
void print_table(Table table);
Table* find_table(vector<Table>& tables, string table_name);
string remove_char_from_string(string s, char c);
vector <Condition> condition_finder(string str);
int find_string_in_string_vector(vector<string> v, string s);
bool condition_checking(Table table, vector<Condition> conditions,int row_index);
Table create_table_by_conditions(Table table, vector <Condition> conditions);
