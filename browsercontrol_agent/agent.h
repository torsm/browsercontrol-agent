#pragma once
#include <jni.h>
#include <jvmti.h>

JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *param, void *reserved);

void JNICALL
cbVMInit(jvmtiEnv *jvmti, JNIEnv *jni, jthread thread);

void JNICALL
cbBreakpoint(jvmtiEnv *jvmti, JNIEnv *jni, jthread thread, jmethodID method, jlocation location);
