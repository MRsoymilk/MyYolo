import argparse

def inside_tool1_kernel(input_file: str, output_file: str):
    dwInterval = 100

    # 读取输入文件
    with open(input_file, "rb") as f:
        buf = bytearray(f.read())

    dwBufLen = len(buf)
    dwPakNum = dwBufLen // dwInterval

    # 处理缓冲区
    for i in range(dwPakNum):
        buf[i * dwInterval] = ~buf[i * dwInterval] & 0xFF  # 按位取反并保持范围

    # 写入输出文件
    with open(output_file, "wb") as f:
        f.write(buf)

def main():
    # 命令行参数解析
    parser = argparse.ArgumentParser(description="Process a binary file by modifying its content.")
    parser.add_argument("--input", required=True, help="Path to the input binary file")
    parser.add_argument("--output", required=True, help="Path to the output binary file")

    args = parser.parse_args()

    # 调用核心处理函数
    inside_tool1_kernel(args.input, args.output)

if __name__ == "__main__":
    main()

