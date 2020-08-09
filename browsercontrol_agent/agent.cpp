#pragma once
#include <cstring>
#include "agent.h"


static jmethodID loadMethod;


JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *, void *) {
	jvmtiEnv *jvmti;
	vm->GetEnv((void **)&jvmti, JVMTI_VERSION_1);

	jvmtiCapabilities capabilities{};
	capabilities.can_access_local_variables = 1;
	capabilities.can_generate_breakpoint_events = 1;
	capabilities.can_force_early_return = 1;
	jvmti->AddCapabilities(&capabilities);

	jvmtiEventCallbacks callbacks{};
	callbacks.VMInit = cbVMInit;
	callbacks.Breakpoint = cbBreakpoint;
	jvmti->SetEventCallbacks(&callbacks, (jint)sizeof(callbacks));

	jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread)nullptr);
	jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, (jthread)nullptr);

	return JNI_OK;
}


void JNICALL
cbVMInit(jvmtiEnv *jvmti, JNIEnv *jni, jthread thread) {
	jclass systemClass = jni->FindClass("java/lang/System");
	loadMethod = jni->GetStaticMethodID(systemClass, "load", "(Ljava/lang/String;)V");
	jvmti->SetBreakpoint(loadMethod, 0);
	jni->DeleteLocalRef(systemClass);
}


void JNICALL
cbBreakpoint(jvmtiEnv *jvmti, JNIEnv *jni, jthread thread, jmethodID method, jlocation location) {
	if (method == loadMethod)
	{
		jobject stackObject;
		jvmti->GetLocalObject(thread, 0, 0, &stackObject);
		const char *filename = jni->GetStringUTFChars((jstring)stackObject, nullptr);

		if (strstr(filename, "browsercontrol") != nullptr)
		{
			jvmti->ForceEarlyReturnVoid(thread);
		}

		jni->ReleaseStringUTFChars((jstring)stackObject, filename);
		jni->DeleteLocalRef(stackObject);
	}
}
