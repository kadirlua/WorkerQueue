#include <workerqueue/WorkerQueue.h>
#include <cstdlib>
#include <filesystem>
#include <jni.h>
#include <string>
#include <vector>

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_sdk_workerqueue_MainActivity_listDirectoryContents(
	JNIEnv* env,
	jobject /* this */,
	jstring path)
{
	const char* pathStr = env->GetStringUTFChars(path, nullptr);

	sdk::concurrency::WorkerQueue wQueue;
	std::vector<std::string> files;
	wQueue.push([&files, pathStr]() {
		namespace fs = std::filesystem;

		for (const auto& entry : fs::directory_iterator(pathStr)) {
			files.push_back(entry.path().filename().string());
		}
	});
	// wait until worker queue has finished
	wQueue.wait();

	jobjectArray fileArray = env->NewObjectArray(files.size(), env->FindClass("java/lang/String"), nullptr);
	for (std::size_t i = 0; i < files.size(); ++i) {
		env->SetObjectArrayElement(fileArray, i, env->NewStringUTF(files[i].c_str()));
	}

	env->ReleaseStringUTFChars(path, pathStr);
	return fileArray;
}