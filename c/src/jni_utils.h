#ifndef JNI_UTILS_H
#define JNO_UTILS_H
#include <jni.h>

/**
 * @brief 创建新的对象
 */
jobject jni_utils_allocate_new_object_by_classname(
	JNIEnv *env,
	jboolean *hasException,
	const char *classname,
	const char *descriptor,
	...
);

/**
 * @brief 调用某个对象的方法
 * From the book The Java Native Interface
 * Programmer's Guide and Specification
 * by Sheng Liang
 */
jvalue jni_utils_call_method_by_name(
	JNIEnv *env,
	jboolean *hasException,
	jobject obj,
	const char *name,
	const char *descriptor,
	...
);

#endif
