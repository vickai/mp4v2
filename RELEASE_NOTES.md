# mp4v2 静态单文件二进制 — 发行说明

> 基于 [enzo1982/mp4v2](https://github.com/enzo1982/mp4v2)（mp4v2.org，fork 自 sandreas/mp4v2，原始项目为 Google Code 的 mp4v2）构建。
> 目标平台：Ubuntu 24.04 / x86_64（amd64）。静态链接 libmp4v2，解压即可直接运行，无需安装任何依赖。

## 一、包含的工具（共 10 个）

| 工具 | 用途 |
| --- | --- |
| `mp4info` | 查看 MP4 / M4A / M4B 文件信息 |
| `mp4tags` | 读写 iTunes 风格元数据标签，支持排序字段（sortname / sortalbum 等） |
| `mp4chaps` | 章节的导入、导出与管理 |
| `mp4art` | 封面图（cover art）的提取与写入 |
| `mp4extract` / `mp4file` / `mp4subtitle` / `mp4track` / `mp4trackdump` | 数据提取、文件编辑、字幕、轨道管理与转储等工具 |
| **`mp4freetag`** | **（本版新增）** 写入任意 iTunes 风格 freeform 标签（`----:com.apple.iTunes:...`） |

## 二、本版亮点：mp4freetag（有声书系列标签）

**背景**：有声书通常不按字母序发行（如《哈利·波特》系列），播放器默认按文件名或标题排序时顺序会乱。行业通行做法（MP3Tag / Audible / m4b-tool 约定）是写入 `----:com.apple.iTunes:SERIES` 与 `----:com.apple.iTunes:SERIES-PART` 这两个 freeform 标签，支持该约定的播放器即可按系列正确排序。

**痛点**：ffmpeg 的 `use_metadata_tags` 写的是 ISO `mdta` 键值对（Apple 播放器不识别）；AtomicParsley 标签集固定、无法写任意 freeform 标签。`mp4freetag` 基于 libmp4v2 的 iTMF 通用接口（`MP4ItmfAddItem`）直接写入，实测写入后 ffprobe 可读、`----` 原子结构正确、已有章节不受影响。

**用法**：

```bash
# 写入系列名与分册号（与 m4b-tool --series / --series-part 语义一致）
./bin/mp4freetag '----' com.apple.iTunes SERIES "Harry Potter" book.m4b
./bin/mp4freetag '----' com.apple.iTunes SERIES-PART "2" book.m4b

# 配合 mp4tags 写入 Apple 标准系列标签与排序字段，双保险
./bin/mp4tags -song "Harry Potter and the Chamber of Secrets" \
              -S "Harry Potter" -M 2 \
              -sortname "Harry Potter 2 - Harry Potter and the Chamber of Secrets" \
              -sortalbum "Harry Potter 2" \
              book.m4b
```

## 三、使用方法

1. 下载本 Release 的 `mp4v2-static-ubuntu2404-amd64.tar.gz` 并解压；
2. 将 `bin/` 目录加入 `PATH`，或直接使用绝对路径调用；
3. 验证安装：

```bash
./bin/mp4info --version
./bin/mp4freetag '----' com.apple.iTunes SERIES "哈利·波特" 测试.m4b && ffprobe -v quiet -show_entries format_tags 测试.m4b
```

## 四、构建与兼容性信息

- **构建方式**：`autoreconf -fiv && ./configure --prefix="$PWD/out" --enable-static --disable-shared && make -j$(nproc) && make install`
- **链接方式**：静态链接 libmp4v2（不依赖系统 libmp4v2.so）；动态链接系统 glibc / libstdc++，与 Ubuntu 24.04、22.04 等主流发行版兼容。
- **零依赖构建（可选）**：如需完全静态（不依赖任何动态库），构建时给 configure 追加 `LDFLAGS="-static"`，产物为 `statically linked`。
- **许可证**：Mozilla Public License 1.1（与上游 mp4v2 一致）。

## 五、相关项目

- [m4b-tool](https://github.com/sandreas/m4b-tool)：有声书合并/整理工具，其 `--series / --series-part` 参数即对应上述 SERIES / SERIES-PART 标签约定；
- [enzo1982/mp4v2](https://github.com/enzo1982/mp4v2)：本二进制的上游源码（mp4v2.org 维护版）。
