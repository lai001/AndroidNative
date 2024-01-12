task("fmt")
do
    on_run(function()
        import("core.project.config")
        import("core.base.option")
        config.load()
        local files = {}
        table.join2(files, os.files("native/cpp/src/**.cpp"))
        table.join2(files, os.files("native/cpp/include/**.h"))
        table.join2(files, os.files("app/src/main/cpp/**.cpp"))

        for _, file in ipairs(files) do
            local cmd = format("clang-format --style=file -i %s", file)
            os.exec(cmd)
        end
    end)
    set_menu {
    }
end

task("java_signature")
do
    on_run(function()
        import("core.project.config")
        import("core.base.option")
        config.load()
        os.mkdir("build")
        local jar = config.get("android_sdk") .. "/platforms/android-33/android.jar"
        local class = "java.io.InputStream"
        local cmd = format("javap -s -public --class-path %s %s ", jar, class)
        local outdata, errdata = os.iorun(cmd)
        if errdata == nil or errdata == "" then
            io.writefile(format("./build/%s.signature", class), outdata)
        end
    end)
    set_menu {
    }
end

task("jni_header")
do
    on_run(function()
        import("core.project.config")
        import("core.base.option")
        config.load()
        os.mkdir("build")
        local jar = config.get("android_sdk") .. "/platforms/android-33/android.jar"
        local java = "./app/src/main/java/com/lai001/androidnative/Native.java"
        local cmd = format(
            "javac -encoding utf8 -d build -h build --source-path %s %s",
            jar, java)
        os.exec(cmd)
    end)
    set_menu {
    }
end

task("build_all")
do
    on_run(function()
        for _, arch in ipairs({ "arm64-v8a", "armeabi-v7a", "x86", "x86_64" }) do
            os.exec(format("xmake f -p android --ndk=D:/Android/Sdk/ndk/25.1.8937393 --android_sdk=D:/Android/Sdk -m debug -k shared -a %s", arch))
            os.exec("xmake")
            os.exec("xmake i")
        end
    end)
    set_menu {
    }
end

target("AndroidNative")
do
    set_kind("shared")
    set_languages("cxx11")
    set_targetdir("app/src/main/jniLibs/x86")
    add_files("native/cpp/src/*.cpp")
    add_headerfiles("native/cpp/include/*.h")
    add_includedirs("native/cpp/include")
    add_includedirs(".xmake/deps/spdlog/include")
    add_rpathdirs("./")
    set_optimize("none")
    set_symbols("debug")
    add_cxxflags("-g", { force = true })
    add_ldflags("-g", { force = true })
    on_install(function (target)
        local plat = get_config("plat")
        local mode = get_config("mode")
        local arch = get_config("arch")
        
        os.cp(target:targetfile(), format("app/src/main/jnilibs/%s/", arch))
    end)
end
