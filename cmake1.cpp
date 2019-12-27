#include <unistd.h>
#include <iostream>

int main(int argc, char** argv)
{
std::cout<<"Running cmake with arguments: ";
for (int i=0;i<argc;++i)
std::cout<<argv[i]<<" ";
std::cout<<std::endl;
sleep(1);
std::cout<<"Done"<<std::endl;
return 0;
}
