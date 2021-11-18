# BMYBBS Java

本项目包含一些使用 Java 语言开发的程序，以及 JNI 接口调用。

## 依赖

1. CMake 3.10+
2. JDK 8+
3. maven

## 构建

```bash
# libbmybbs-java.so
pushd c
cp config.sample.cmake config.cmake
# 修改 config.cmake
mkdir build
pushd build
cmake ..
make
popd
popd

# bmybbs-index-${version}.jar
pushd java-indes
mvn package

# dependencies
mvn dependency:copy-dependencies
```

## 许可

MIT

## 版本日志

* 0.1.0: 重新实现原 PyLucene 脚本中的索引 / 查询方式。（少许字段追加、改动）

