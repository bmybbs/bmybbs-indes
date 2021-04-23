#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "jni_utils.h"
#include "config.h"
#include "ytht/fileop.h"
#include "bmy/convcode.h"

#define YTHTBBS_ATTACH_HEADER "beginbinaryattach "
static jstring getContent(JNIEnv *env, const char *filename) {
	struct mmapfile mf = { .ptr = NULL };
	char *content_gbk, *content_utf;
	jstring jstr_content = NULL;
	unsigned int attach_size;
	size_t i, j, m, n, line, pos;
	bool is_first_line;
	if (mmapfile(filename, &mf) >= 0) {
		if ((content_gbk = malloc(mf.size)) != NULL) {
			if ((content_utf = malloc(mf.size * 2)) != NULL) {
				// skip content header
				i = 0;
				j = 0;
				line = 0;
				while (i < mf.size && line < 4) {
					if (mf.ptr[i++] == '\n') {
						line++;
					}
				}

				is_first_line = true;
				while (i < mf.size) {
					if (is_first_line || mf.ptr[i] == '\n') {
						is_first_line = false;

						if (mf.ptr[i] == '\n') {
							content_gbk[j++] = mf.ptr[i++];
						}

						// skip content footer
						if (mf.size - i > 3 && strncmp(mf.ptr + i, "--\n", 3) == 0) {
							break;
						}

						if (mf.size - i > 18 && strncmp(mf.ptr + i, YTHTBBS_ATTACH_HEADER, strlen(YTHTBBS_ATTACH_HEADER)) == 0) {
							m = i + strlen(YTHTBBS_ATTACH_HEADER);
							if (m < mf.size) {
								n = m;
								while (n < mf.size - 6) {
									if (mf.ptr[n] == '\n') {
										if (mf.ptr[n + 1] != 0) {
											break;
										}

										// 存在附件
										pos = n + 2;
										memcpy(&attach_size, mf.ptr + pos, sizeof(unsigned int));
										attach_size = ntohl(attach_size);

										i = pos + 4 + attach_size;
										break;
									} else {
										n++;
									}
								}
							}
						}
					} else {
						// 普通内容，直接拷贝
						// TODO skip 其他
						content_gbk[j++] = mf.ptr[i++];
					}
				}

				g2u(content_gbk, strlen(content_gbk), content_utf, mf.size * 2);
				jstr_content = (*env)->NewStringUTF(env, content_utf);
				free(content_utf);
			}
			free(content_gbk);
		}
		mmapfile(NULL, &mf);
	}
	return jstr_content;
}

/*
 * Class:     edu_xjtu_bmybbs_ythtbbs_Article
 * Method:    getContent
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_edu_xjtu_bmybbs_ythtbbs_Article_getContent(JNIEnv *env, jobject jThis) {
	jclass clazz;
	jfieldID fid;
	jstring jstr_board, jstr_filename, jstr_content = NULL;
	const char *board, *filename;
	char buf[STRLEN * 2];

	clazz = (*env)->GetObjectClass(env, jThis);
	fid = (*env)->GetFieldID(env, clazz, "board", "Ljava/lang/String;");
	if (fid != NULL) {
		jstr_board = (*env)->GetObjectField(env, jThis, fid);
		board = (*env)->GetStringUTFChars(env, jstr_board, NULL);
		fid = (*env)->GetFieldID(env, clazz, "filename", "Ljava/lang/String;");
		if (fid != NULL) {
			jstr_filename = (*env)->GetObjectField(env, jThis, fid);
			filename = (*env)->GetStringUTFChars(env, jstr_filename, NULL);

			snprintf(buf, sizeof(buf), MY_BBS_HOME "/boards/%s/%s", board, filename);
			jstr_content = getContent(env, buf);

			(*env)->ReleaseStringUTFChars(env, jstr_filename, filename);
			(*env)->DeleteLocalRef(env, jstr_filename);
		}

		(*env)->ReleaseStringUTFChars(env, jstr_board, board);
		(*env)->DeleteLocalRef(env, jstr_board);
	}
	(*env)->DeleteLocalRef(env, clazz);
	return jstr_content;
}

