#include "UIThread.h"
#include "cocos2d.h"
#include <pthread.h>
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
using namespace cocos2d;
#define JavaRunnable "java/lang/Runnable"
#define UIHelper "com/x4enjoy/UIHelper"
#define MainActivity "com/x4enjoy/MainActivity"
#define Activity "android/app/Activity"
extern "C"
{
void Java_com_x4enjoy_UIHelper_run(JNIEnv*  env, jobject thiz)
{
    UIThread::getInstance().performNextTask();
}

}

UIThread& UIThread::getInstance()
{
    if(_instance == 0)
    {
        _instance = new UIThread;
    }
    return *_instance;
}

void UIThread::runInUIThread(RunnablePtr task)
{
    Runnable* task_ptr = task.release();
    mutexLock();
    _task_queue.push(task_ptr);
    mutexUnlock();

    jobject java_task = _env->NewObject(_UIHelper, _UIHelper_Construct);
    _env->CallVoidMethod(_main_activity, _runOnUiThread, java_task);
    _env->DeleteLocalRef(java_task);
}
void UIThread::performNextCocosTask(float)
{
    bool repeat = false;
    Runnable* task = 0;
    mutexLock();
    if(_cocos_queue.size() > 0)
    {
        task = _cocos_queue.front();
        _cocos_queue.pop();

        if(_cocos_queue.size() > 0)
            repeat = true;
    }
    mutexUnlock();

    if(task != 0)
    {
        task->run();



        delete task;
    }
    if(!repeat)
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UIThread::performNextCocosTask), this);
}

void UIThread::runInCocos2dThread(RunnablePtr task)
{

    Runnable* task_ptr = task.release();
    mutexLock();
    _cocos_queue.push(task_ptr);
    mutexUnlock();

    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UIThread::performNextCocosTask), this, 0, false);
}

void UIThread::performNextTask()
{
    Runnable* task = 0;
    mutexLock();
    if(_task_queue.size() > 0)
    {
        task = _task_queue.front();
        _task_queue.pop();
    }
    mutexUnlock();

    if(task != 0)
    {
        task->run();



        delete task;
    }
}

UIThread::UIThread()
{
    JniMethodInfo getActivity;
    if(!JniHelper::getStaticMethodInfo(
            getActivity, MainActivity,
                "getActivity", F(J(Activity), None)))
    {

    }

    _env = getActivity.env;
    jclass mainActivity = getActivity.classID;
    jmethodID mainActivity_getActivity = getActivity.methodID;

    _main_activity = _env->NewGlobalRef(_env->CallStaticObjectMethod(mainActivity, mainActivity_getActivity));

    jclass activity_class = JniHelper::getClassID(Activity, _env);
    _runOnUiThread = _env->GetMethodID(activity_class, "runOnUiThread", F(Void, J(JavaRunnable)));

    _UIHelper = (jclass)_env->NewGlobalRef(JniHelper::getClassID(UIHelper, _env));
    _UIHelper_Construct = _env->GetMethodID(_UIHelper, "<init>", F(Void, None));


}

void UIThread::mutexLock()
{
    pthread_mutex_lock(&mutex);

}
void UIThread::mutexUnlock()
{
    pthread_mutex_unlock(&mutex);

}

UIThread* UIThread::_instance = 0;
