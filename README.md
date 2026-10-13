# cjson

一个用 C 写的 JSON 解析练习项目。它会读取 JSON 文件，先把内容切分成 token，再递归解析成 `JsonValue` 树；你可以访问对象成员、数组元素，也可以打印解析结果。

这是我的第一个项目之一，目前主要用于学习 C 语言、词法分析、递归解析和动态内存管理。它还不是一个适合处理不可信输入的完整 JSON 库；已知限制见下文，后续计划见 [改进建议](IMPROVEMENTS.md)。

## 编译和运行

需要 GCC 和 Make。在项目根目录运行：

```sh
make
./test test_small.json
```

`test` 是演示程序，里面会读取 `test_small.json` 中的特定字段。想单独查看词法分析或语法解析的输出，可以运行：

```sh
make lexer_test parser_test
./lexer/lexer_test test_small.json
./parser/parser_test test_small.json
```

清理编译产物：`make clean`。

## 基本用法

对外接口在 [`cjson.h`](cjson.h)，数据类型在 [`jsonvalue.h`](jsonvalue.h)。例如读取项目自带的样例文件：

```c
#include <stdio.h>
#include "cjson.h"

int main(void)
{
    JsonValue *root = jsonvalue_parse("test_small.json");
    JsonValue *name = get_object_value(root, "name");

    if (name != NULL && name->tag == JSON_STRING)
        printf("name: %s\n", name->str);

    pretty_print(root);
    jsonvalue_free(root);
    return 0;
}
```

常用接口：

| 函数 | 用途 |
| --- | --- |
| `jsonvalue_parse(filename)` | 读取文件并生成 `JsonValue` 树 |
| `get_object_value(value, key)` | 取得对象中指定键的值 |
| `get_array_value(value, index)` | 取得数组中指定下标的值 |
| `get_value(value, key1, ..., NULL)` | 按多级对象键查找；最后必须传入 `NULL` |
| `pretty_print(value)` | 打印解析结果 |
| `jsonvalue_free(value)` | 释放解析结果；当前版本仍有内存释放问题 |

## 项目结构

```text
lexer/       读取文件并生成 token
parser/      将 token 解析为 JsonValue 树，并负责打印和释放
cjson.c/h    对外提供的解析和查询接口
jsonvalue.h  JSON 值的数据结构
test.c       演示程序
test_*.json  示例输入
```

## 当前限制

- 解析器尚未严格检查所有 JSON 语法，部分非法输入可能被接受。
- 字符串转义尚未完整处理；打印结果也不能保证是合法的 JSON 文本。
- 错误处理和内存释放仍需完善。请先使用可信的示例文件，不要将当前版本直接用于生产环境。
- `cjson.h` 中声明的 `create_array`、`create_object` 尚未实现。

具体问题和建议的修复顺序记录在 [IMPROVEMENTS.md](IMPROVEMENTS.md)。
