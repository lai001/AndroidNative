
#ifndef native_cpp_include_IO_InputStream_h
#define native_cpp_include_IO_InputStream_h

#include <jni.h>

namespace an
{

class InputStream
{
  public:
    struct Methods
    {
        jmethodID read;
        jmethodID skip;
        jmethodID reset;
        jmethodID close;
        jmethodID available;
    };

  private:
    JNIEnv *env;
    jobject object;
    Methods methods;

    int64_t curPostion;
    int fileLength;

    InputStream &operator=(const InputStream &) = delete;
    InputStream(const InputStream &) = delete;

  public:
    InputStream(JavaVM *jvm, JNIEnv *jenv, jobject jobj);
    ~InputStream();

    int available() const;
    int read(unsigned char *buf, const int bufSize);

    void reset();
    void close();
    int64_t skip(int64_t num);

    int64_t seek(int64_t pos);
};

} // namespace an

#endif