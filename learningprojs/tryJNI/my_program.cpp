#include <iostream>
#include <jni.h>

int main() {
    // Initialize the Java Virtual Machine (JVM)
    JavaVM *jvm;
    JNIEnv *env;

    JavaVMInitArgs vmArgs;
    JavaVMOption options[1];
    options[0].optionString = "-Djava.class.path=</Users/jayshah/Documents/programming/cppwork/learningprojs/tryJNI>";  // Set the classpath
    vmArgs.version = JNI_VERSION_1_8;
    vmArgs.nOptions = 1;
    vmArgs.options = options;
    vmArgs.ignoreUnrecognized = JNI_FALSE;

    JNI_CreateJavaVM(&jvm, (void**)&env, &vmArgs);

    // Find the class
    jclass javaClass = env->FindClass("MyJavaClass");

    if (javaClass != nullptr) {
        // Find the method
        jmethodID methodId = env->GetMethodID(javaClass, "printMessage", "(Ljava/lang/String;)V");

        if (methodId != nullptr) {
            // Call the method
            jstring message = env->NewStringUTF("Hello from C++!");
            env->CallVoidMethod(javaClass, methodId, message);
            env->DeleteLocalRef(message);
        } else {
            std::cerr << "Method not found" << std::endl;
        }

        // Release the class reference
        env->DeleteLocalRef(javaClass);
    } else {
        std::cerr << "Class not found" << std::endl;
    }

    // Destroy the JVM
    jvm->DestroyJavaVM();

    return 0;
}
