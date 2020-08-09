#pragma once
#include "nativeadvert_browsercontrol.h"

JNIEXPORT void JNICALL 
Java_nativeadvert_browsercontrol_resize0(JNIEnv *, jclass, jint, jint) {
	// Empty stub
}

JNIEXPORT void JNICALL 
Java_nativeadvert_browsercontrol_navigate0(JNIEnv *, jclass, jstring) {
	// Empty stub
}

JNIEXPORT jboolean JNICALL 
Java_nativeadvert_browsercontrol_browsercontrol0(JNIEnv *, jclass, jobject, jstring) {
	// Empty stub
	return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_nativeadvert_browsercontrol_destroy0(JNIEnv *, jclass) {
	// Empty stub
}
