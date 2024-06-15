#include <jni.h>
#include <string>
#include <concurrency/WorkerQueue.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_sdk_workerqueue_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string result;
    {
        sdk::concurrency::WorkerQueue wQueue;
        wQueue.push([&result](){
            result.append("Test Message!");
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return env->NewStringUTF(result.c_str());
}