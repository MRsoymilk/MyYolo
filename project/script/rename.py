import os
import argparse

def rename_files_in_directory(input_folder, prefix, suffix):
    # 遍历目录中的所有文件
    for filename in os.listdir(input_folder):
        # 获取文件的完整路径
        file_path = os.path.join(input_folder, filename)

        # 如果是文件而不是目录
        if os.path.isfile(file_path):
            # 获取文件名和扩展名
            base_filename, ext = os.path.splitext(filename)

            # 如果没有前缀则添加前缀
            if not base_filename.startswith(prefix):
                base_filename = prefix + base_filename
            
            # 如果没有后缀则添加后缀
            if not base_filename.endswith(suffix):
                base_filename = base_filename + suffix
            
            # 创建新文件名
            new_filename = base_filename + ext
            new_file_path = os.path.join(input_folder, new_filename)

            # 如果新文件名与原文件名不同，则重命名
            if file_path != new_file_path:
                os.rename(file_path, new_file_path)
                print(f"Renamed {filename} to {new_filename}")

def main():
    # 创建参数解析器
    parser = argparse.ArgumentParser(description="Rename files by adding a prefix and/or suffix.")
    
    # 添加命令行参数
    parser.add_argument('--input_folder', type=str, required=True, help='Path to the input folder')
    parser.add_argument('--prefix', type=str, default='', help='Prefix to add to the filenames')
    parser.add_argument('--suffix', type=str, default='', help='Suffix to add to the filenames')

    # 解析命令行参数
    args = parser.parse_args()

    # 调用文件重命名函数
    rename_files_in_directory(args.input_folder, args.prefix, args.suffix)

if __name__ == "__main__":
    main()
