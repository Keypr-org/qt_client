#include <QtTest/QtTest>

#include "../src/nativeMessaging/nativemessaging.h"

#ifdef Q_OS_WIN
#include <io.h>
#else
#include <unistd.h>
#endif

class NativeMessagingTest : public QObject {
    Q_OBJECT

private slots:
    void stop_unblocksReadLoopQuickly_whenBlockedOnStdin();
};

/**
 * @brief Regression test for a slow app shutdown: NativeMessaging::readMessages()
 * runs in a background thread blocked on a stdin read with no natural way to
 * interrupt it as long as Chrome keeps the Native Messaging pipe open.
 * NativeMessaging::stop() must close that descriptor and join the thread
 * quickly instead of the app hanging on quit.
 */
void NativeMessagingTest::stop_unblocksReadLoopQuickly_whenBlockedOnStdin() {
    int pipeFds[2];

#ifdef Q_OS_WIN
    QVERIFY(_pipe(pipeFds, 4096, 0) == 0);
    const int savedStdin = _dup(0);
    QVERIFY(_dup2(pipeFds[0], 0) == 0);
    _close(pipeFds[0]);
#else
    QVERIFY(pipe(pipeFds) == 0);
    const int savedStdin = dup(0);
    QVERIFY(dup2(pipeFds[0], 0) == 0);
    close(pipeFds[0]);
#endif

    NativeMessaging nativeMessaging;
    nativeMessaging.start();

    // Give the worker thread time to actually park in the blocking read.
    QTest::qWait(200);

    QElapsedTimer timer;
    timer.start();
    nativeMessaging.stop();
    const qint64 elapsedMs = timer.elapsed();

#ifdef Q_OS_WIN
    _dup2(savedStdin, 0);
    _close(savedStdin);
    _close(pipeFds[1]);
#else
    dup2(savedStdin, 0);
    close(savedStdin);
    close(pipeFds[1]);
#endif

    QVERIFY2(elapsedMs < 1000, qPrintable(QString("stop() took %1ms").arg(elapsedMs)));
}

QTEST_MAIN(NativeMessagingTest)
#include "tst_nativemessaging.moc"
