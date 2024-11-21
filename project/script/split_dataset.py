import os
import argparse
import random
from math import ceil

def split_dataset(dir_in, dir_out, train_ratio=0.8, val_ratio=0.1, test_ratio=0.1, shuffle=True):
    # 用于存储所有符合条件的文件路径
    all_files = []

    # 递归遍历目录及子目录
    for root, _, files in os.walk(dir_in):
        for file in files:
            if file.endswith('.jpg'):  # 只处理 .jpg 文件
                # 获取文件的完整路径并添加到列表
                all_files.append(os.path.join(root, file))
    
    # 打乱文件顺序
    if shuffle:
        random.shuffle(all_files)
    
    # 计算每一部分的大小
    total_files = len(all_files)
    train_size = ceil(total_files * train_ratio)
    val_size = ceil(total_files * val_ratio)
    test_size = total_files - train_size - val_size  # 剩下的作为测试集

    # 切分数据集
    train_files = all_files[:train_size]
    val_files = all_files[train_size:train_size + val_size]
    test_files = all_files[train_size + val_size:]

    # 将每个部分的文件路径写入对应的文件
    write_to_file(os.path.join(dir_out, 'train.txt'), train_files)
    write_to_file(os.path.join(dir_out, 'val.txt'), val_files)
    write_to_file(os.path.join(dir_out, 'test.txt'), test_files)

    print(f"Dataset split: {train_size} train, {val_size} validation, {test_size} test.")

def write_to_file(file_path, files):
    # 读取文件内容
    if os.path.exists(file_path):
        with open(file_path, 'r', encoding='utf-8') as f:
            existing_paths = set(f.read().splitlines())  # 读取已存在的路径并存入集合

    else:
        existing_paths = set()  # 如果文件不存在，初始化为空集合

    with open(file_path, 'a', encoding='utf-8') as f:
        for file in files:
            # 获取文件的绝对路径
            absolute_path = os.path.abspath(file)
            
            # 只有在路径不在现有路径中时才写入
            if absolute_path not in existing_paths:
                f.write(absolute_path + '\n')
                existing_paths.add(absolute_path)  # 将新路径添加到现有路径集合中

def str2bool(value):
    if value.lower() in ['true', '1', 't', 'y', 'yes']:
        return True
    elif value.lower() in ['false', '0', 'f', 'n', 'no']:
        return False
    else:
        raise argparse.ArgumentTypeError("Boolean value expected.")

def main():
    # 设置命令行参数解析器
    parser = argparse.ArgumentParser(description='Split dataset into train, val, and test sets.')
    parser.add_argument('--dir_in', type=str, required=True, help='The directory to search for dataset files')
    parser.add_argument('--dir_out', type=str, required=True, help='The directory to write dataset files')
    parser.add_argument('--train-ratio', type=float, default=0.8, help='The ratio of train set')
    parser.add_argument('--val-ratio', type=float, default=0.1, help='The ratio of validation set')
    parser.add_argument('--test-ratio', type=float, default=0.1, help='The ratio of test set')
    parser.add_argument('--shuffle', type=str2bool, default=True, help="Whether to shuffle the data (default is True)")

    # 解析命令行参数
    args = parser.parse_args()

    # 调用函数进行数据集拆分
    split_dataset(args.dir_in, args.dir_out, args.train_ratio, args.val_ratio, args.test_ratio, args.shuffle)

if __name__ == '__main__':
    main()
