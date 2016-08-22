#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sstream>
#include <map>
#include "inverted_index.h"
#include "color.h"
using namespace std;

#define RESOURSE_DIR "../resource/"

int main(int argc, char const *argv[])
{
	struct dirent *pDirEntry = NULL;
    DIR *pDir = NULL;
    if((pDir = opendir(RESOURSE_DIR)) == NULL)
    {
       printf(L_RED"err_msg:"YELLOW" director "RESOURSE_DIR" not exists\n"NONE);
       return -1;
    }

    InvertedIndex* inverted_index = new InvertedIndex();

    while((pDirEntry = readdir(pDir)) != NULL )
    {  
        if (strcmp(pDirEntry->d_name, ".") == 0 || strcmp(pDirEntry->d_name, "..") == 0)
        {
            continue;
        }
        stringstream ss;
        ss<<RESOURSE_DIR<<pDirEntry->d_name;
        inverted_index->StatWords(ss.str().c_str());
        printf("file stat success: %s\n", ss.str().c_str());
    }
    closedir(pDir);

    map<string,map<string,int> > result = inverted_index->result_index();
    map<string,map<string,int> >::iterator it;

    for(it = result.begin(); it != result.end(); ++it)
    {
    	printf(L_RED"%s"NONE":", (it->first).c_str());
    	map<string,int> file_count = it->second;
    	map<string,int>::iterator file_it;
    	for(file_it = file_count.begin(); file_it != file_count.end(); ++file_it)
    	{
    		printf(YELLOW"["NONE"%s,%d"YELLOW"]"NONE, (file_it->first).c_str(), file_it->second);
    	}
    	printf("\n");
    }
	return 0;
}
