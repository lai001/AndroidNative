#include "IO/InputStream.h"
#include <string>
#include <thread>
#include <algorithm>
#include "spdlog/spdlog.h"

an::InputStream::InputStream(JavaVM *jvm, JNIEnv *jenv, jobject jobj)
{
    env = jenv;
    jclass javaClass = jenv->GetObjectClass(jobj);
    object = jenv->NewGlobalRef(jobj);

    methods.read = env->GetMethodID(javaClass, "read", "([B)I");
    methods.skip = env->GetMethodID(javaClass, "skip", "(J)J");
    methods.reset = env->GetMethodID(javaClass, "reset", "()V");
    methods.close = env->GetMethodID(javaClass, "close", "()V");
    methods.available = env->GetMethodID(javaClass, "available", "()I");
    reset();
    fileLength = available();
}

an::InputStream::~InputStream()
{
    env->DeleteGlobalRef(object);
}

int an::InputStream::available() const
{
    jint available = env->CallIntMethod(object, methods.available);
    return available;
}

int an::InputStream::read(unsigned char *buf, const int bufSize)
{
    jbyteArray byteArray = env->NewByteArray(bufSize);
    const jint read = env->CallIntMethod(object, methods.read, byteArray);
    jbyte *data = env->GetByteArrayElements(byteArray, nullptr);
    env->ReleaseByteArrayElements(byteArray, data, 0);
    memcpy(buf, data, std::min(read, bufSize));
    env->DeleteLocalRef(byteArray);
    curPostion += read;
    return read;
}

void an::InputStream::reset()
{
    curPostion = 0;
    env->CallVoidMethod(object, methods.reset);
}

void an::InputStream::close()
{
    curPostion = 0;
    env->CallVoidMethod(object, methods.close);
}

int64_t an::InputStream::skip(int64_t num)
{
    curPostion += num;
    return env->CallLongMethod(object, methods.skip, num);
}

int64_t an::InputStream::seek(int64_t pos)
{
    reset();
    curPostion = std::max((int64_t)0, std::min((int64_t)fileLength, pos));
    curPostion = skip(curPostion);
    return curPostion;
}
