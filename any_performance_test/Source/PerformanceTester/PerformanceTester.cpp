#include "PerformanceTester.h"
#include <atomic>
#include <map>
#include <array>
#include <chrono>
#include <functional>
#include <any>
#include "../any/Include/any.h"

#define NMS_UNUSED(x) ((void)x)

using namespace std;
using namespace chrono;

atomic<uint> g_new_calls = 0;
atomic<uint> g_allocated_bytes = 0;
atomic<uint> g_delete_calls = 0;

void* operator new(std::size_t sz)
{
    g_new_calls++;
    g_allocated_bytes += sz;
    return std::malloc(sz);
}
void operator delete(void* ptr) noexcept
{
    g_delete_calls++;
    std::free(ptr);
}

struct big_struct
{
    double a[128];
};

struct test_data
{
    test_data() {}
    test_data(uint new_calls, uint allocated_bytes, uint delete_calls, const chrono::nanoseconds& duration) :
        m_new_calls(new_calls),
        m_allocated_bytes(allocated_bytes),
        m_delete_calls(delete_calls),
        m_duration(duration)
    {
    }
    uint m_new_calls = 0;
    uint m_allocated_bytes = 0;
    uint m_delete_calls = 0;
    chrono::nanoseconds m_duration = 0ns;
};

template<class _any>
test_data testAssignBIG_SMALL(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    for (uint i = 0; i < testCount; i++)
    {
        _any a;
        a = big_struct();
        a = string("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
        a = string("cba");
        a = 0;
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

template<class _any>
test_data testAssignSMALL_BIG(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    for (uint i = 0; i < testCount; i++)
    {
        _any a;
        a = 0;
        a = string("cba");
        a = string("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
        a = big_struct();
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveCORRECT_SMALL_STD(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    int b;
    for (uint i = 0; i < testCount; i++)
    {
        std::any a;
        a = 12;
        b = any_cast<int>(a);
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveCORRECT_BIG_STD(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    big_struct b;
    for (uint i = 0; i < testCount; i++)
    {
        std::any a;
        a = big_struct();
        b = any_cast<big_struct>(a);
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveINCORRECT_SMALL_STD(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    float b;
    for (uint i = 0; i < testCount; i++)
    {
        std::any a;
        a = 12;
        try {
            b = any_cast<float>(a);
        }
        catch (std::exception& exc)
        {
            NMS_UNUSED(exc);
        }
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveINCORRECT_BIG_STD(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    big_struct b;
    for (uint i = 0; i < testCount; i++)
    {
        std::any a;
        a = string();
        try {
            b = any_cast<big_struct>(a);
        }
        catch (std::exception& exc)
        {
            NMS_UNUSED(exc);
        }
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveCORRECT_SMALL_NMS(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    int b;
    for (uint i = 0; i < testCount; i++)
    {
        nms::any a;
        a = 12;
        b = a.value<int>();
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveCORRECT_BIG_NMS(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    big_struct b;
    for (uint i = 0; i < testCount; i++)
    {
        nms::any a;
        a = big_struct();
        b = a.value<big_struct>();
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveINCORRECT_SMALL_NMS(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    float b;
    for (uint i = 0; i < testCount; i++)
    {
        nms::any a;
        a = 12;
        b = a.value<float>();
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

test_data testRetrieveINCORRECT_BIG_NMS(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    big_struct b;
    for (uint i = 0; i < testCount; i++)
    {
        nms::any a;
        a = string();
        b = a.value<big_struct>();
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

template<class _any>
test_data testCOPY_SMALL(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    _any b;
    for (uint i = 0; i < testCount; i++)
    {
        _any a;
        a = 0;
        b = a;
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

template<class _any>
test_data testCOPY_BIG(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    _any b;
    for (uint i = 0; i < testCount; i++)
    {
        _any a;
        a = big_struct();
        b = a;
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

template<class _any>
test_data testMOVE_SMALL(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    _any b;
    for (uint i = 0; i < testCount; i++)
    {
        _any a;
        a = 0;
        b = std::move(a);
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

template<class _any>
test_data testMOVE_BIG(const uint& testCount)
{
    test_data data;

    g_new_calls = 0;
    g_allocated_bytes = 0;
    g_delete_calls = 0;

    auto start = high_resolution_clock::now();

    _any b;
    for (uint i = 0; i < testCount; i++)
    {
        _any a;
        a = big_struct();
        b = std::move(a);
    }

    auto end = high_resolution_clock::now();

    data.m_new_calls = g_new_calls;
    data.m_allocated_bytes = g_allocated_bytes;
    data.m_delete_calls = g_delete_calls;
    data.m_duration = (end - start);

    return data;
}

std::array<uint, 8> g_test_steps = { 10000, 100000 };
using test_function = function<test_data(const uint&)>;
std::map<string, pair<test_function, test_function>> g_tests = {
    { "Assign big-small", std::make_pair(testAssignBIG_SMALL<std::any>, testAssignBIG_SMALL<nms::any>) },
{ "Assign small-big", std::make_pair(testAssignSMALL_BIG<std::any>, testAssignSMALL_BIG<nms::any>) },
{ "Retrieve correct small",   std::make_pair(testRetrieveCORRECT_SMALL_STD, testRetrieveCORRECT_SMALL_NMS) },
{ "Retrieve correct big",    std::make_pair(testRetrieveCORRECT_BIG_STD, testRetrieveCORRECT_BIG_NMS) },
{ "Retrieve incorrect small",    std::make_pair(testRetrieveINCORRECT_SMALL_STD, testRetrieveINCORRECT_SMALL_NMS) },
{ "Retrieve incorrect big",   std::make_pair(testRetrieveINCORRECT_BIG_STD, testRetrieveINCORRECT_BIG_NMS) },
{ "Copy small",    std::make_pair(testCOPY_SMALL<std::any>, testCOPY_SMALL<nms::any>) },
{ "Copy big",   std::make_pair(testCOPY_BIG<std::any>, testCOPY_BIG<nms::any>) },
{ "Move small",   std::make_pair(testMOVE_SMALL<std::any>, testMOVE_SMALL<nms::any>) },
{ "Move big",   std::make_pair(testMOVE_BIG<std::any>, testMOVE_BIG<nms::any>) },
};

CPerformanceTester::CPerformanceTester(QWidget* parent):
    QWidget(parent)
{
    setWindowTitle("nms::any vs std::any");

    m_RootLayout = new QVBoxLayout;
    m_TestsProgresBar = new QProgressBar;
    m_DoTests = new QPushButton;
    m_TestClassesTabs = new QTabWidget;

    m_TestClassesTabs->setMinimumSize(QSize(600, 400));
    m_TestsTabs[ETestSubject::NewCalls] = new QTabWidget;
    m_TestsTabs[ETestSubject::AllocatedMemory] = new QTabWidget;
    m_TestsTabs[ETestSubject::TimeTaken] = new QTabWidget;
    m_TestClassesTabs->addTab(m_TestsTabs[ETestSubject::NewCalls], "operator new calls");
    m_TestClassesTabs->addTab(m_TestsTabs[ETestSubject::AllocatedMemory], "allocated memory");
    m_TestClassesTabs->addTab(m_TestsTabs[ETestSubject::TimeTaken], "time taken");
    m_RootLayout->addWidget(m_TestClassesTabs);

    m_TestsProgresBar->setMinimum(0);
    m_TestsProgresBar->setMaximum(100);
    m_RootLayout->addWidget(m_TestsProgresBar);

    m_DoTests->setText("Do tests");
    QObject::connect(m_DoTests, &QPushButton::released, this, &CPerformanceTester::doTests);
    m_RootLayout->addWidget(m_DoTests);

    setLayout(m_RootLayout);
}

CPerformanceTester::~CPerformanceTester()
{
}

void CPerformanceTester::doTests()
{
    m_TestsTabs[ETestSubject::NewCalls]->clear();
    m_TestsTabs[ETestSubject::AllocatedMemory]->clear();
    m_TestsTabs[ETestSubject::TimeTaken]->clear();

    int progressMax = g_tests.size()*g_test_steps.size();
    int progress = 0;
    m_TestsProgresBar->setValue(progress);
    for (auto& test :g_tests)
    {
        auto chartNewCalls = new QChart();
        auto chartAllocatedBytes = new QChart();
        auto chartTimeTaken = new QChart();

        QLineSeries* seriesNewCallsSTD = new QLineSeries();
        seriesNewCallsSTD->setName("std::any new calls");
        QLineSeries* seriesNewCallsNMS = new QLineSeries();
        seriesNewCallsNMS->setName("nms::any new calls");

        QLineSeries* seriesAllocatedBytesSTD = new QLineSeries();
        seriesAllocatedBytesSTD->setName("std::any allocated bytes");
        QLineSeries* seriesAllocatedBytesNMS = new QLineSeries();
        seriesAllocatedBytesNMS->setName("nms::any allocated bytes");

        QBarSeries* seriesTimeTakenSTD = new QBarSeries();
        seriesTimeTakenSTD->setName("any time taken");

        QBarSet* setTimeTakenSTD = new QBarSet("std");
        QBarSet* setTimeTakenNMS = new QBarSet("nms");
        QStringList categories;

        int step = 1;
        for (auto& step :g_test_steps)
        {
            auto data_std = test.second.first(step);
            auto data_nms = test.second.second(step);

            seriesNewCallsSTD->append(QPointF(step, data_std.m_new_calls));
            seriesNewCallsNMS->append(QPointF(step, data_nms.m_new_calls));

            seriesAllocatedBytesSTD->append(QPointF(step, data_std.m_allocated_bytes));
            seriesAllocatedBytesNMS->append(QPointF(step, data_nms.m_allocated_bytes));

            setTimeTakenSTD->append(data_std.m_duration.count() / 1000.0);
            setTimeTakenNMS->append(data_nms.m_duration.count() / 1000.0);

            categories << QString::number(step);

            progress++;
            m_TestsProgresBar->setValue(progress);
        }

        seriesTimeTakenSTD->append(setTimeTakenSTD);
        seriesTimeTakenSTD->append(setTimeTakenNMS);

        chartNewCalls->addSeries(seriesNewCallsSTD);
        chartNewCalls->addSeries(seriesNewCallsNMS);

        chartAllocatedBytes->addSeries(seriesAllocatedBytesSTD);
        chartAllocatedBytes->addSeries(seriesAllocatedBytesNMS);

        chartTimeTaken->addSeries(seriesTimeTakenSTD);

        //new calls
        {
            QValueAxis *axisY = new QValueAxis();
            axisY->setTitleText("new calls");
            axisY->setLabelFormat("%i");
            axisY->setTickCount(seriesNewCallsSTD->count());
            chartNewCalls->addAxis(axisY, Qt::AlignLeft);
            seriesNewCallsSTD->attachAxis(axisY);
            seriesNewCallsNMS->attachAxis(axisY);

            QValueAxis *axisX = new QValueAxis();
            axisX->setTitleText("tests count");
            axisX->setLabelFormat("%g");
            axisX->setMinorTickCount(-1);
            chartNewCalls->addAxis(axisX, Qt::AlignBottom);
            seriesNewCallsSTD->attachAxis(axisX);
            seriesNewCallsNMS->attachAxis(axisX);
        }

        //allocated memory
        {
            QValueAxis *axisY = new QValueAxis();
            axisY->setTitleText("allocated memory [B]");
            axisY->setLabelFormat("%i");
            axisY->setTickCount(seriesAllocatedBytesSTD->count());
            chartAllocatedBytes->addAxis(axisY, Qt::AlignLeft);
            seriesAllocatedBytesSTD->attachAxis(axisY);
            seriesAllocatedBytesNMS->attachAxis(axisY);

            QValueAxis *axisX = new QValueAxis();
            axisX->setTitleText("tests count");
            axisX->setLabelFormat("%g");
            axisX->setMinorTickCount(-1);
            chartAllocatedBytes->addAxis(axisX, Qt::AlignBottom);
            seriesAllocatedBytesSTD->attachAxis(axisX);
            seriesAllocatedBytesNMS->attachAxis(axisX);
        }

        //time taken
        {
            QBarCategoryAxis *axis = new QBarCategoryAxis();
            axis->append(categories);
            chartTimeTaken->createDefaultAxes();
            chartTimeTaken->setAxisX(axis, seriesTimeTakenSTD);
        }

        m_TestsTabs[ETestSubject::NewCalls]->addTab(new QChartView(chartNewCalls), QString::fromStdString(test.first));
        m_TestsTabs[ETestSubject::AllocatedMemory]->addTab(new QChartView(chartAllocatedBytes), QString::fromStdString(test.first));
        m_TestsTabs[ETestSubject::TimeTaken]->addTab(new QChartView(chartTimeTaken), QString::fromStdString(test.first));
    }
    m_TestsProgresBar->setValue(m_TestsProgresBar->maximum());
}
