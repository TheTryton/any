#pragma once

#include <QtWidgets>
#include <QtCharts>

enum class ETestSubject
{
    NewCalls,
    AllocatedMemory,
    TimeTaken
};

class CPerformanceTester : public QWidget
{
    Q_OBJECT
public:
    CPerformanceTester(QWidget* parent = nullptr);
    ~CPerformanceTester();
private slots:
    void doTests();
private:
    QPushButton *       m_DoTests = nullptr;
    QProgressBar *      m_TestsProgresBar = nullptr;
    QVBoxLayout*        m_RootLayout = nullptr;
    QTabWidget*         m_TestClassesTabs;
    QMap<ETestSubject,QTabWidget*>  m_TestsTabs;
};

