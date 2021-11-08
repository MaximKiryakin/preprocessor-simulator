#include <stdio.h>
#include <string.h>


enum{MAX_LEN_IN = 81, MAX_LEN_OUT = 10001, MAX_MANE = 33};

char ArrayName[MAX_MANE][MAX_MANE];
char ArrayNameValue[MAX_MANE][MAX_MANE];
char str_in[MAX_LEN_IN];

int  cntName = 0;

//обработка переменной
void SetName(char* name,char* value)
{
    for(int i = 0; i < cntName;i++)
    {
        if(strcmp((char*)ArrayName[i],name))
            continue;

        strcpy(ArrayNameValue[i],value);
        return;
    }
    strcpy(ArrayName[cntName],name);
    strcpy(ArrayNameValue[cntName],value);
    cntName++;
}

//получение индекса максимального имени
int get_index_name_step(char* str)
{
    int indexMax = -1;
    int lenMax;
    int len;
    for(int i = 0; i < cntName;i++)
    {
        char* name = ArrayName[i];

        char* pIn = strstr(str,name);

        if(pIn != str)
            continue;

        if(indexMax < 0) {
            indexMax = i;
            continue;
        }
        lenMax = (int)strlen(ArrayName[indexMax]);
        len = (int)strlen(ArrayName[i]);

        if(lenMax < len)
            indexMax = i;
    }

    return indexMax;
}

//получение индекса максимального имени
int get_index_name(char* str,char* delta)
{
    *delta = 0;
    int indexMax = get_index_name_step(str);
    while(indexMax < 0 && *(str + *delta - 1) != '$')
    {
        //поиск позиции символа в строке
        char* pIn = strstr(str, "$");

        if(!pIn)
            break;

        indexMax = get_index_name_step(pIn+1);
        str++;
        *delta += 1;
    }

    return indexMax;
}

//обработка случая подстановки
void inser_name_in_string(char* name,char* value,char* str)
{
    char strTmp[MAX_LEN_OUT];

    int lenName = (int)strlen(name);

    //1.копирую
    strcpy(strTmp,str + lenName + 1);

    //2.вставляю в строку значение
    strcpy(str,value);

    //3.вставляю конец строки
    int lenVal = strlen(value);
    strcat(str+lenVal,strTmp);
}

//преобразование строки
int str_transfer(char* str){
    //1.проверка на комментарий
    if (*str == '#')
        return -1;

    char * pChar = str;

    //2.проверка на переменную
    if ((*pChar >= 'a' && *pChar <= 'z') || (*pChar >= 'A' && *pChar <= 'Z')) {
        while ((*pChar >= 'a' && *pChar <= 'z') || (*pChar >= 'A' && *pChar <= 'Z')) {
            pChar++;
        }
        //цикл закончился - анализируем результат
        int bOk = 1;
        if (*pChar == '=') {
            if(strlen(pChar))
            {
                if(*(pChar+1) == '=')
                    bOk = 0;
            }

            if(bOk)
            {
                char name[32];
                strncpy(name,str,pChar - str);
                name[pChar - str] = '\0';

                char value[81];
                strcpy(value,pChar+1);
                value[strlen(pChar)-2] = '\0';

                //обработка переменной
                SetName(name, value);

                return 0;
            }
        }
    }

    //3.проверка на подстановки
    pChar = str;
    while (*pChar != '\0') {
        if(*pChar == '$') {
            //получение индекса максимального имени
            char delta = 0;
            int index = get_index_name(pChar+1,&delta);
            if(index != -1) {
                //нашли имя сделали подстановку
                inser_name_in_string(ArrayName[index],ArrayNameValue[index],pChar+delta);
                return 1;
            }
        }
        pChar++;
    }
    //это строка
    return 2;
}

int main(void) {
    int nLine = 0;
    while(fgets(str_in,MAX_LEN_IN, stdin) != NULL){

        int bRez = str_transfer(str_in);

        //преобразование строки
        //выполняю подставновку пока могу
        while(bRez == 1){
            bRez = str_transfer(str_in);
        }
        //не смог найти подстановку
        if(bRez == 2)
            printf("%s",str_in);

        nLine++;
    }//цикл ввода данных
}
