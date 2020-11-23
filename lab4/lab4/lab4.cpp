// lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <thread>
int l() {

    return 1;

}

typedef void (*Work_Callback_Function)(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work);
void ThreadWork1(PTP_CALLBACK_INSTANCE instance, PTP_WORK work) {
    //auto* myThis = reinterpret_cast<ThreadpoolQueueProcessor*>(parameter);

    //queueElementHandler(myThis);
}
void ThreadWork(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work) {
    //auto* myThis = reinterpret_cast<ThreadpoolQueueProcessor*>(parameter);

    //queueElementHandler(myThis);
}
int main()
{
    DWORD max = 3;
//    TP_POOL* ptp = CreateThreadpool(NULL);
//    SetThreadpoolThreadMinimum(ptp, 1);
//    SetThreadpoolThreadMaximum(ptp, max);
///    TP_CALLBACK_ENVIRON tpce;
//    TpInitializeCallbackEnviron(&tpce);
    Work_Callback_Function fff = ThreadWork;
    void (*Work_Callback_Function)(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work) = ThreadWork;
    void (*Work_Callback_Function1)(PTP_CALLBACK_INSTANCE instance, PTP_WORK work) = ThreadWork1;
    //------------------------------
//    PTP_CALLBACK_INSTANCE f=NULL;
//    PTP_WORK work = CreateThreadpoolWork((PTP_WORK_CALLBACK)Work_Callback_Function, NULL,&tpce);
    //CreateThreadpoolWork(l,)
    

    //-----------------------------0--
    /*PTP_WORK
WINAPI
CreateThreadpoolWork(
    _In_ PTP_WORK_CALLBACK pfnwk,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe
    );
    */
//    TP_CALLBACK_ENVIRON* ptpce;
 //   std::thread tr;
//
    return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
