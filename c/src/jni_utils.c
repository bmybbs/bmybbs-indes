#include <string.h>
#include <jni.h>

jobject jni_utils_allocate_new_object_by_classname(
	JNIEnv *env,
	jboolean *hasException,
	const char *classname,
	const char *descriptor,
	...
) {
	va_list args;
	jclass clazz = NULL;
	jmethodID cid = NULL;
	jobject result = NULL;

	if ((*env)->EnsureLocalCapacity(env, 2) == JNI_OK) {
		clazz = (*env)->FindClass(env, classname);
		if (clazz == NULL)
			goto END;

		cid = (*env)->GetMethodID(env, clazz, "<init>", descriptor);
		if (cid == NULL)
			goto END;

		va_start(args, descriptor);
		result = (*env)->NewObjectV(env, clazz, cid, args);
		va_end(args);
	}

	if (hasException) {
		*hasException = (*env)->ExceptionCheck(env);
	}

END:
	if (clazz)
		(*env)->DeleteLocalRef(env, clazz);
	return result;
}

jvalue jni_utils_call_method_by_name(
	JNIEnv *env,
	jboolean *hasException,
	jobject obj,
	const char *name,
	const char *descriptor,
	...
) {
	va_list args;
	jclass clazz;
	jmethodID mid;
	jvalue result;
	memset(&result, 0, sizeof(jvalue));

	if ((*env)->EnsureLocalCapacity(env, 2) == JNI_OK) {
		clazz = (*env)->GetObjectClass(env, obj);
		mid = (*env)->GetMethodID(env, clazz, name, descriptor);

		if (mid) {
			const char *p = descriptor;
			while (*p != ')') p++;
			p++; /* skip ')' */

			va_start(args, descriptor);
			switch(*p) {
			case 'V':
				(*env)->CallVoidMethodV(env, obj, mid, args);
				break;
			case '[':
			case 'L':
				result.l = (*env)->CallObjectMethodV(env, obj, mid, args);
				break;
			case 'Z':
				result.z = (*env)->CallBooleanMethodV(env, obj, mid, args);
				break;
			case 'B':
				result.b = (*env)->CallByteMethodV(env, obj, mid, args);
				break;
			case 'C':
				result.c = (*env)->CallCharMethodV(env, obj, mid, args);
				break;
			case 'S':
				result.s = (*env)->CallShortMethodV(env, obj, mid, args);
				break;
			case 'I':
				result.i = (*env)->CallIntMethodV(env, obj, mid, args);
				break;
			case 'J':
				result.j = (*env)->CallLongMethodV(env, obj, mid, args);
				break;
			case 'F':
				result.f = (*env)->CallFloatMethodV(env, obj, mid, args);
				break;
			case 'D':
				result.d = (*env)->CallDoubleMethodV(env, obj, mid, args);
				break;
			default:
				(*env)->FatalError(env, "illegal descriptor");
			}
			va_end(args);
		}
		(*env)->DeleteLocalRef(env, clazz);
	}

	if (hasException) {
		*hasException = (*env)->ExceptionCheck(env);
	}

	return result;
}

/*
static jvalue jniutils_call_methodV(
	JNIEnv *env,
	jboolean *hasException,
	jclass clazz,
	jmethodID mid,
	const char *descriptor,
	va_list args
) {
}
*/

