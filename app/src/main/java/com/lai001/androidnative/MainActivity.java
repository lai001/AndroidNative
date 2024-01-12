package com.lai001.androidnative;

import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import com.lai001.androidnative.databinding.ActivityMainBinding;

import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    static {
        System.loadLibrary("native");
        Native.setJVM();
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        try {
            InputStream inputStream = getAssets().open("test.bin");
            Native.setInputStream(inputStream);
        } catch (IOException e) {
            Log.d(TAG, "onCreate: " + e);
        }

    }
}