#include <QtTest/QtTest>

class TestExample : public QObject
{
    Q_OBJECT

private slots:
    void addition();
    void stringComparison();
};

void TestExample::addition()
{
    QCOMPARE(1 + 2, 3);
}

void TestExample::stringComparison()
{
    QCOMPARE(QString("hello"), QString("hello"));
}

QTEST_MAIN(TestExample)

#include "tst_simple_test.moc"