#include <map>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "inverted_index.h"
using namespace std;

int InvertedIndex::StatWords(const char* file_name)
{
	FILE *fp;   
    if((fp = fopen(file_name,"r")) == NULL)
    { 
        printf("cannot open %s", file_name);
        return -1;
    }
    
    char str_line[MAXLINE];
	while(fgets(str_line, MAXLINE, fp) != NULL)
	{
		int len = strlen(str_line);
		str_line[len-1] = '\0';  /*去掉换行符*/
		ParseWordsByLine(str_line,file_name);
	} 
    fclose(fp);
    return 0; 
}


int InvertedIndex::ParseWordsByLine(char* str_line, const char* file_name)
{
	if(strlen(str_line) == 0)
	{
		return -1;
	}

	const char* needle=" \n\t";
	/*strtok在s中查找包含在delim中的字符并用NULL(‘\0’)来替换，直到找遍整个字符串。 
	返回值：从s开头开始的一个个被分割的串。当没有被分割的串时则返回NULL。*/
	char* word = strtok(str_line, needle);
	while(word != NULL)
	{
		InsertWordToMap(word,file_name);
	    word = strtok(NULL, needle);
	}
	return 0;
}

void InvertedIndex::InsertWordToMap(char* word, const char* file_name)
{
	if(strlen(word) == 0)
	{
		return;
	}
	string word_str = word;
	string file_name_str = file_name;

	map<string,map<string,int> >::iterator it;
	it = result_index_.find(word_str);

	if(it == result_index_.end())//not found
	{
		map<string,int> file_word_count;
		file_word_count.insert(pair<string,int>(file_name_str,1));
		result_index_[word_str] = file_word_count;
	}
	else
	{
		map<string,int> file_word_count = result_index_[word_str];
		file_word_count[file_name_str] ++ ;
		result_index_[word_str] = file_word_count;
	}
}