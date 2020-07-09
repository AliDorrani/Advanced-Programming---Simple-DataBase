#include "CA1.h"

using namespace std;

vector < string > split (string str , string delimiters )
{
    vector <string> result;
    string word;
    int cotation_ignore_delimiters = 1;
    for (int i=0 ; i<str.size() ; i++)
    {
        // when double cotation is seen delimiters must be ignored
        if (str[i] == '\"')
            cotation_ignore_delimiters *= -1;
        else if ( delimiters.find(str[i]) < delimiters.size() && cotation_ignore_delimiters != -1)
        {
            if (word.size() == 0)
                continue;
            else
            {
                result.push_back(word);
                word.clear();
            }
        }
        else
            word.push_back(str[i]);
    }
    //making sure last word is pushed back
    if (word.size()>0)
        result.push_back(word);

    return result ;
}

Table create_table(string table_name, vector<string> headers)
{
    //creating table
    Table table;
    //setting table name
    table.name = table_name;
    // setting headers
    table.data.push_back(headers);
    return table;
}

bool insert(Table &table, vector<string> row_info)
{
    //checking validity of the data given
    if (row_info.size() == table.data[0].size())
    {
        table.data.push_back(row_info);
        return false;
    }
    else
        return true;
}

void print_table(Table table)
{
    if (table.data.size() < 2)
    {
        cout << "Empty set" << endl;
        return ;
    }

    //evaluating max spaces needed for printing spaces
    vector <int> max_spaces ;
    for (int i = 0; i < table.data[0].size(); i++)
    {
        max_spaces.push_back(table.data[0][i].size());
        for (int j = 1; j < table.data.size(); j++)
            if (max_spaces[i] < table.data[j][i].size())
                max_spaces[i] = table.data[j][i].size();
        //cout << max_spaces[i] << " ";
    }
    //cout << endl;
    //return ;

    //printing table
    for (int i = 0; i < table.data.size(); i++)
    {
        if (i < 2)
        {
            for (int j = 0; j < table.data[0].size(); j++)
            {
                cout << "+";
                for (int k = 0; k < max_spaces[j] + 2; k++)
                    cout << "-";
            }
            cout << "+";
            cout << endl;
        }
        cout << "|";
        for (int j = 0; j < table.data[i].size(); j++)
        {
            cout << " " << table.data[i][j];
            for (int k = 0; k < (max_spaces[j] - table.data[i][j].size() + 1); k++)
                cout << " " ;
            cout << "|";
        }
        cout << endl;
    }
    for (int j = 0; j < table.data[0].size(); j++)
    {
        cout << "+";
        for (int k = 0; k < max_spaces[j] + 2; k++)
            cout << "-";
    }
    cout << "+";
    cout << endl;
    cout << table.data.size() - 1 << " rows in set." <<endl;
}

Table* find_table(vector<Table>& tables, string table_name)
{
    for (int i = 0;i < tables.size(); i++)
        if (tables[i].name == table_name)
        {
            //cout << "table : " << tables[i]
            return &(tables[i]);
        }
    return NULL;
}

string remove_char_from_string(string s, char c)
{
    string ss;
    for (int i = 0; i < s.size(); i++)
        if (s[i] != c)
            ss+=s[i];
    return ss;
}

vector <Condition> condition_finder(string str)
{
    vector <Condition> conditions;
    int where_pos = str.find("WHERE");
    //cout << where_pos;
    int conditions_start_pos = where_pos + 5;

    string word;
    string delimiters = ", ;";
    int cotation_ignore_delimiters = 1;
    for (int i = conditions_start_pos ; i < str.size() ; i++)
    {
        if (delimiters.find(str[i]) < delimiters.size() )
            continue;
        int j = i;
        while (str[j] != ',' && str[j] != ';')
            j++;
        Condition condition;
        string sub = str.substr(i,j);
        if (sub.find('\"') < sub.size() )
        {
            condition.type = SPECEFIED_COLOUMN_DATA;
            sub = remove_char_from_string(sub, ' ');
            sub = remove_char_from_string(sub,';');
            int equal_sign_pos = sub.find('=');
            vector <string> temp  = split(sub, "= ,;");
            if (sub[equal_sign_pos+1] == '\"')
            {
                condition.b = temp[1];
                condition.a = temp[0];
            }
            else if (sub[equal_sign_pos-1] = '\"')
            {
                condition.b = temp[0];
                condition.a = temp[1];
            }
            else {
                //cout << "wth" << endl;
            }
        }
        else
        {
            condition.type = COLOUMNS_DATA_MATCH;
            vector<string> temp = split(sub, " =");
            condition.a = temp[0];
            condition.b = temp[1];
        }
        i += j;
        conditions.push_back(condition);
    }

    //for (int i = 0; i < conditions.size(); i++)
    //    cout << conditions[i].type << " " << conditions[i].a << " " << conditions[i].b << endl;

    return conditions;
}

int find_string_in_string_vector(vector<string> v, string s)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i] == s)
            return i;
    return -1;
}


bool condition_checking(Table table, vector<Condition> conditions,int row_index)
{
    for (int i = 0; i < conditions.size(); i++)
    {
        if (conditions[i].type == SPECEFIED_COLOUMN_DATA)
        {
            int coloumn_index = find_string_in_string_vector(table.data[0], conditions[i].a);
            if (coloumn_index == -1)
                return false;
            if (table.data[row_index][coloumn_index] != conditions[i].b)
                return false;
        }
        else if (conditions[i].type == COLOUMNS_DATA_MATCH)
        {
            int coloumn_index1 = find_string_in_string_vector(table.data[0], conditions[i].a);
            if (coloumn_index1 == -1)
                return false;
            int coloumn_index2 = find_string_in_string_vector(table.data[0], conditions[i].b);
            if (coloumn_index2 == -1)
                return false;

            if (table.data[row_index][coloumn_index1] != table.data[row_index][coloumn_index2])
                return false;
        }
    }
    return true;
}

Table create_table_by_conditions(Table table, vector <Condition> conditions)
{
    Table t = create_table("temp", table.data[0]);
    for (int i = 1; i < table.data.size(); i++)
    {
        if (condition_checking(table, conditions, i) == true)
        {
            insert(t, table.data[i]);
        }
    }
    return t;
}


int main()
{
    vector < string > inputCommand;
    string str;
    // vector for table stored in program
    vector<Table> tables;

    while (true)
    {
        //getting command
        getline( cin , str);
        // spliting command by splitter
        inputCommand = split ( str , " ,();" );

        if (inputCommand[0] == "CREATE")
        {
            //capturing table name
            string table_name = inputCommand[2];
            // erasing CREATE TABLE and tablename from input COMMAND
            for (int i = 1; i < 4; i++)
                inputCommand.erase(inputCommand.begin());
            // erasing useless texts
            for (int i = 1; i < inputCommand.size(); i++)
                inputCommand.erase(inputCommand.begin() + i);

            Table* table = find_table(tables, table_name);
            if (table != NULL)
                cout << "table already exists" << endl;
            else
            {
                tables.push_back(create_table(table_name, inputCommand));
                cout << "Query OK!" << endl;
            }
        }
        else if (inputCommand[0] == "INSERT")
        {
            //cleaning up the input command to give it to insert function
            string table_name = inputCommand[2];
            for (int i = 0; i < 4; i++)
                inputCommand.erase(inputCommand.begin());

            Table* table = find_table(tables, table_name);
            if (table == NULL)
            {
                cout << "table not found" << endl;
            }
            else if (insert(*table, inputCommand) == false)
            {
                //cout << table->data.size() << endl;
                cout << "Query OK!" << endl;
            }
            else
            {
                //cout << table->name << endl;
                cout << "Error happend." << endl;

            }
        }
        else if (inputCommand[0] == "SELECT")
        {
            // cleaning up inputCommand
            Table* table = find_table(tables, inputCommand[3]);
            if (table == NULL)
                cout << "table not found" << endl;
            else if (inputCommand.size() < 5)
            {
                print_table(*table);
            }
            else if (inputCommand[4] == "WHERE")
            {
                vector <Condition> conditions = condition_finder(str);
                Table t = create_table_by_conditions(*table, conditions);
                print_table(t);
            }
            else
            {
                cout << "INVALID COMMAND.";
            }
        }
        else if (inputCommand[0] == "QUIT")
        {
            break;
        }
        else {
            cout << "COMMAND NOT FOUND" << endl;
        }
    }

    return 0;
}
