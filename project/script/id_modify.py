import os
import argparse

def modify_txt_files(directory, number):
    # 遍历目录下的所有文件
    for filename in os.listdir(directory):
        # 只处理 .txt 文件
        if filename.endswith('.txt'):
            txt_path = os.path.join(directory, filename)
            
            # 打开文件并读取内容
            try:
                # 尝试打开文件并读取内容
                with open(txt_path, 'r', encoding='utf-8') as file:
                    lines = file.readlines()
            except (UnicodeDecodeError, IOError) as e:
                # 捕获编码错误或文件I/O错误，并打印错误信息，继续执行
                print(f"Error reading file {filename}: {e}")
                continue

            # 如果文件为空，则跳过
            if not lines:
                print(f"Skipping empty file: {filename}")
                continue

            # 如果文件不为空，修改每一行
            modified_lines = []
            for line in lines:
                parts = line.split()
                # 如果这一行有 5 个元素，按要求修改第一个元素
                if len(parts) == 5 and parts[0] == '0':
                    parts[0] = str(number)  # 修改为指定的数字
                    modified_lines.append(" ".join(parts) + '\n')
                    # 将修改后的内容写回文件
                    print(f"Modified file: {filename}")
                    with open(txt_path, 'w', encoding='utf-8') as file:
                        file.writelines(modified_lines)
                    break
                else:
                    print(f"Skipping no target file: {filename}")
                    break

def main():
    # 设置命令行参数解析器
    parser = argparse.ArgumentParser(description='Modify .txt files in a directory.')
    parser.add_argument('--directory', type=str, required=True, help='Directory to search for .txt files')
    parser.add_argument('--number', type=int, required=True, help='Number to replace 0 with')

    # 解析命令行参数
    args = parser.parse_args()

    # 调用函数进行文件修改
    modify_txt_files(args.directory, args.number)

if __name__ == '__main__':
    main()
