package com.lai001.androidnative;

import java.io.InputStream;

public class Native {
    public static native void setJVM();

    public static native void setInputStream(InputStream inputStream);

}
