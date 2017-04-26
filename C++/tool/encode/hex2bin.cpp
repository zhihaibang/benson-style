/*
* 二进制转化为十六进制字符串，长度变为原来的两倍
* 二进制转化为Base64，长度增加1/3
* 不管从性能还是转化体积，Base64都比十六进制字符串要好
*/
/*16进制字符串和二进制的转换*/
int GetNum(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    else
        return -1;
}

//16进制string转换成二进制
string HexStringToString(const string &source_str)
{
    string dest_str;
    int len = source_str.size();
    uint8_t tmp;
    int number;
    for (int i = 0; i < len; ++i)
    {
        number = GetNum(source_str[i]);
        tmp = number * 16;
        i++;
        if (i >= len)
        {
            number = GetNum('0');
        }
        else
        {
            number = GetNum(source_str[i]);
        }
        tmp += number;
        dest_str.append((char *)&tmp, sizeof(tmp));
    }
    return dest_str;
}

// string转换成16进制的string
string StringToHexString(const string &source_str)
{
    string dest_str;
    int len = source_str.size();
    char tmp[3];
    for (int i = 0; i < len; ++i)
    {
        sprintf(tmp, "%x%x", (source_str[i] >> 4) & 0xf, source_str[i] & 0xf);
        dest_str.append(tmp, 2);
    }
    return dest_str;
}




