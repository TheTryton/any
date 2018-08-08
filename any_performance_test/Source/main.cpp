#include <iostream>
#include <any>
#include <string>
#include <chrono>
#include <atomic>
#include <vector>
#include <array>
#include <map>
#include <functional>
#include <QApplication>
#include "PerformanceTester/PerformanceTester.h"
#include "../any/Include/any.h"

using namespace std;
using namespace chrono;


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    CPerformanceTester performanceTester;
    performanceTester.show();
    /*for (auto& test : g_tests)
    {
        cout << endl << endl;
        cout << test.first << endl;
        for (auto& step : g_test_steps)
        {
            auto data_std = test.second.first(step);
            auto data_nms = test.second.second(step);

            cout << "STD:" << endl;
            printTestData(data_std);
            cout << "NMS:" << endl;
            printTestData(data_nms);
        }
    }
    cin.get();*/
    return application.exec();
}