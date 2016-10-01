/*
    Программа для иллюстрации работы двух нитей исполнения.
    Каждая нить исполнения просто увеличивает на 1
    разделяемую переменную а.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // exit
#include <unistd.h> // sleep

int a = 0;

/* 
   Переменная а является глобальной статической для всей
   программы, поэтому будет разделяться обеими нитями
   исполнения 
*/

/*
    Ниже следует текст функции, которая будет ассоциирована
    со 2-м thread`oм
*/

void* my_thread(void* dummy) {
/* 
    У функции должна быть такая сигнатура, и не важно
    что мы не используем входной параметр и возвращаемое
    значение
*/
    pthread_t my_thread_id; 
    sleep(1);

/*
    Переменная my_thread_id является динамической локальной переменной
    функции my_thread(), т.е. помещается на стеке и, следовательно, не
    разделяется нитями исполнения.
*/

    my_thread_id = pthread_self();
    a += 1;
    printf("Thread %u, result = %d\n" , (unsigned int)my_thread_id , a);
    
    return NULL;
}

/*
    Функция main() - она же ассоциированная функция главного thread`a
*/

int main()
{
    pthread_t thread_id , my_thread_id;
    int result;
    /* 
        Пытаемся создать новую нить исполнения, ассоциированную
        с функцией my_thread(). Передаем ей в качестве параметра
        значение NULL. В случае удачи в переменную thread_id занесется
        id нового thread`a.
        В случае ошибки перекращаем работу.
    */
    
    result = pthread_create(&thread_id , 
                            (pthread_attr_t *)NULL , 
                            my_thread ,
                            NULL);

    if (result) {
        printf("Can`t create thread, returned value = %d\n" , result);
        exit(-1);
    }

    printf("Thread was created, thread id = %u\n" , (unsigned int)thread_id);

    /* Запрашиваем id главного thread`a */
    my_thread_id = pthread_self();
    a += 1;
    printf("Thread %u, result = %d\n" , (unsigned int)my_thread_id , a);

    /*
        Ожидаем завершение порожденного thread`a, не интересуясь, какое значение он
        вернет. Если не выполнить вызов этой функции, то возможно ситуация, когда
        мы завершим функцию main() до того, как выполнится порожденный thread, что
        автоматически повлечет за собой его завершение, исказив результаты
    */

    pthread_join(thread_id , (void **) NULL);
    return 0;
}





































