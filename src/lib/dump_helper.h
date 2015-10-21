#ifndef _DUMP_HELPER_
#define _DUMP_HELPER_


#include <iostream>
#include <map>
template<typename T>
void dump_container( const std::string msg, T container )
{
    std::cout << msg;
    for( auto i= container.begin();
              i!= container.end(); ++i )
        std::cout << *i << " "; 

    std::cout << std::endl;
}

template<typename T1, typename T2>
void dump_map( const std::string msg, const std::map<T1,T2> & m)
{
    std::cout << msg;
    for( auto i= m.begin();
              i!= m.end(); ++i )
        std::cout << i->first << " "; 

    std::cout << std::endl;
}


#endif// _DUMP_HELPER_
