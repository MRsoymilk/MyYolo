import os
import argparse

def get_matching_files(input_folder, txt_filename):
    """查找所有与 txt 文件同名的文件"""
    base_filename = os.path.splitext(txt_filename)[0]
    matching_files = []
    
    for filename in os.listdir(input_folder):
        if filename.startswith(base_filename) and filename != txt_filename:
            matching_files.append(filename)
    
    return matching_files

def should_rename_file(txt_file_path):
    """检查 .txt 文件是否为空"""
    return os.path.exists(txt_file_path) and os.path.getsize(txt_file_path) == 0

def rename_file(input_folder, filename, prefix, suffix):
    """重命名文件：添加前缀和后缀"""
    base_filename = os.path.splitext(filename)[0]
    ext = os.path.splitext(filename)[1]
    
    # 处理前缀：如果文件没有前缀，则添加
    new_filename = base_filename
    if prefix and not new_filename.startswith(prefix):
        new_filename = prefix + new_filename
    
    # 处理后缀：如果文件没有后缀，则添加
    if suffix:
        # 如果文件名已经包含后缀，则不重复添加
        if not new_filename.endswith(suffix):
            new_filename = new_filename + suffix
    
    # 保持扩展名不变
    new_filename = new_filename + ext

    new_file_path = os.path.join(input_folder, new_filename)

    # 如果新文件名与原文件名不同且文件不存在，则重命名
    if os.path.join(input_folder, filename) != new_file_path:
        # 处理文件名冲突，避免覆盖已有文件
        counter = 1
        while os.path.exists(new_file_path):
            new_filename = f"{base_filename}_{counter}"
            if prefix and not new_filename.startswith(prefix):
                new_filename = prefix + new_filename
            if suffix and not new_filename.endswith(suffix):
                new_filename = new_filename + suffix
            new_filename = new_filename + ext
            new_file_path = os.path.join(input_folder, new_filename)
            counter += 1
        
        # 执行重命名
        try:
            old_file_path = os.path.join(input_folder, filename)
            os.rename(old_file_path, new_file_path)
            print(f"Renamed {old_file_path} to {new_file_path}")
        except Exception as e:
            print(f"Error renaming file {old_file_path}: {e}")
    
def rename_files_in_directory(input_folder, prefix, suffix):
    """根据文件夹中的 txt 文件和同名文件进行重命名"""
    for filename in os.listdir(input_folder):
        if filename.endswith(".txt"):
            txt_file_path = os.path.join(input_folder, filename)
            
            # 检查 txt 文件是否为空
            if should_rename_file(txt_file_path):
                # 获取与该 .txt 文件同名的其他文件
                matching_files = get_matching_files(input_folder, filename)
                
                # 先重命名 .txt 文件
                rename_file(input_folder, filename, prefix, suffix)
                
                # 重命名与 .txt 文件同名的其他文件
                for file in matching_files:
                    rename_file(input_folder, file, prefix, suffix)

def main():
    # 创建参数解析器
    parser = argparse.ArgumentParser(description="Rename files based on txt file content.")
    
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
