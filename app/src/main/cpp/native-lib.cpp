#include <jni.h>
#include <string>
#include "AndroidNative.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"

#ifdef __cplusplus
extern "C"
{
#endif

    static JavaVM *jvm = nullptr;

    /*
     * Class:     com_lai001_androidnative_Native
     * Method:    setJVM
     * Signature: ()V
     */
    JNIEXPORT void JNICALL Java_com_lai001_androidnative_Native_setJVM(JNIEnv *jenv, jclass)
    {
        const jint status = jenv->GetJavaVM(&jvm);
        const std::string tag = "native";
        auto android_logger = spdlog::android_logger_mt("android", tag);
        android_logger->set_level(spdlog::level::trace);
        android_logger->flush_on(spdlog::level::trace);
        android_logger->set_pattern("%v");
        spdlog::set_default_logger(android_logger);
    }

    /*
     * Class:     com_lai001_androidnative_Native
     * Method:    setInputStream
     * Signature: (Ljava/io/InputStream;)Ljava/lang/String;
     */
    JNIEXPORT void JNICALL Java_com_lai001_androidnative_Native_setInputStream(JNIEnv *jenv, jclass, jobject jobj)
    {
        an::InputStream* inputStream = new an::InputStream(jvm, jenv, jobj);
        int available = inputStream->available();
        unsigned char buf[4096];
        while (available > 0) {
            available -= inputStream->read(buf, 4096);
            spdlog::trace("{}, {}", "available", available);
        }
    }

#ifdef __cplusplus
}
#endif
