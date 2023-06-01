#include <skiplist.h>

#define PRINT_END cout << "Test End Success =========>\n";

using namespace std;

constexpr int COUNT = 100;

void test1()
{
    // create 
    SkipList<int,string> L;
    
    // insert
    for(int i = 0; i < COUNT; i ++ )
    {
        string s = to_string(i) + "th";
        L.insert_element(i, s);
    }
    
    // search
    for(int i = 0; i < COUNT; i ++ )
    {
        if(i % 5 == 0)
        {
            cout << "[" << i << "] " << L.search_element(i) << endl;
        }
    }
    // repeat insert
    for(int i = 0; i < COUNT / 10; i ++ )
    {
        if(i % 2 == 0)
        {
            string s = to_string(i) + "times";
            L.insert_element(i, s);
            cout << "[" << i << "] " << L.search_element(i) << endl;
        }
    }
    // delete 
    for(int i = 0; i < COUNT / 10; i ++ )
    {
        if(i % 2 == 0)  
            L.delete_element(i);
    }
    // insert
    for(int i = 0; i < COUNT / 10; i ++ )
    {
        if(i % 2 == 0)
        {
            string s = to_string(i) + "times";
            L.insert_element(i, s);
            cout << "[" << i << "] " << L.search_element(i) << endl;
        }
    }
    // size 
    cout << "Size = " << L.size() << endl;
    // display
    L.displat_list();
    // save to disk
    L.dump_file();
}

void test2()
{
    // read disk file into memory
    SkipList<int,string> L;
    L.load_file();
    // display 
    L.displat_list();
    // size 
    cout << "size = " << L.size() << endl;
}

int main()
{
    PRINT_END;
    test1();
    test2();
    return 0;
}