#ifndef UITHREAD_H
#define UITHREAD_H
#include <queue>
#include <memory>
#include "JNI_Library.h"
#include "cocos2d.h"
class Runnable
{
public:
    virtual void run()=0;
};
typedef std::auto_ptr<Runnable> RunnablePtr;


class UIThread : cocos2d::CCObject
{
public:
    static UIThread& getInstance();

    void runInUIThread(RunnablePtr task);
    void runInCocos2dThread(RunnablePtr task);
    void performNextTask();
private:
    UIThread();
    void mutexLock();
    void mutexUnlock();

    void performNextCocosTask(float);

    static UIThread* _instance;
    typedef std::queue<Runnable*> TaskQueue;
    TaskQueue _task_queue;
    TaskQueue _cocos_queue;

    JNIEnv* _env;

    jclass _UIHelper;
    jobject _main_activity;
    jmethodID _runOnUiThread;
    jmethodID _UIHelper_Construct;

};

#endif
