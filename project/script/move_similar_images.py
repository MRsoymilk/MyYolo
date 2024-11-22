import os
import shutil
import argparse
from PIL import Image
import numpy as np
from skimage.metrics import structural_similarity as ssim
from concurrent.futures import ThreadPoolExecutor, as_completed
import sys
import time

# 计算两张图像的SSIM相似度
def calculate_ssim(image1, image2):
    # 将图像转换为灰度
    image1_gray = image1.convert("L")
    image2_gray = image2.convert("L")
    
    # 将灰度图像转换为numpy数组
    image1_gray = np.array(image1_gray)
    image2_gray = np.array(image2_gray)
    
    # 计算SSIM相似度
    score, _ = ssim(image1_gray, image2_gray, full=True)
    return score

# 读取目录下的所有图像文件
def read_images(input_dir):
    image_files = [f for f in os.listdir(input_dir) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]
    image_paths = [os.path.join(input_dir, file) for file in image_files]
    return image_paths

# 将图像列表分成指定大小的批次
def divide_into_batches(image_paths, batch_size):
    return [image_paths[i:i + batch_size] for i in range(0, len(image_paths), batch_size)]

# 处理单个批次，比较其中的图像并标记相似的图像
def process_batch(batch, image_paths, rm_img, batch_img, threshold_ssim, total_count, batch_size, thread_index):
    keep_going = False
    batch_same = True  # 假设当前批次所有图像都相同
    for i, img_path1 in enumerate(batch):
        current_index = image_paths.index(img_path1)  # 当前图像的索引
        if rm_img[current_index]:  # 如果该图像已被标记为删除，跳过
            continue
        for img_path2 in batch[i + 1:]:
            current_index2 = image_paths.index(img_path2)  # 第二张图像的索引
            if rm_img[current_index2]:  # 如果另一个图像已被标记为删除，跳过
                continue

            # 读取两张图像
            image1 = Image.open(img_path1)
            image2 = Image.open(img_path2)
            
            # 计算SSIM
            similarity = calculate_ssim(image1, image2)
            
            # 输出当前批次的处理信息（根据线程ID输出到不同的行）
            sys.stdout.write(f"\033[{thread_index + 2}HT {thread_index + 1} - {batch_size}/{current_index2 + 1}|{current_index + 101}/{total_count} "
                             f"Comparing {os.path.basename(img_path1)} and {os.path.basename(img_path2)} SSIM = {similarity:.4f}")
            sys.stdout.flush()

            if similarity > threshold_ssim:
                rm_img[current_index2] = True
                batch_img[current_index2] = batch.index(img_path1)
                keep_going = True  # 如果有图像被标记为删除，则继续处理
        batch_same = batch_same and batch_img.count(batch.index(img_path1)) == len(batch)

    return keep_going

# 主处理函数
def move_similar_images(input_dir, output_dir, threshold_ssim, batch_size):
    # 读取图像路径
    image_paths = read_images(input_dir)
    total_count = len(image_paths)  # 总图像数量
    rm_img = [False] * len(image_paths)  # 初始时所有图像都不会被删除
    batch_img = [0] * len(image_paths)  # 初始化批次编号

    # 划分批次
    divided_img = divide_into_batches(image_paths, batch_size)

    keep_going = True
    while keep_going:
        keep_going = False
        with ThreadPoolExecutor() as executor:
            # 提交每个批次的任务
            futures = [executor.submit(process_batch, batch, image_paths, rm_img, batch_img, threshold_ssim, total_count, batch_size, i) 
                       for i, batch in enumerate(divided_img)]
            
            try:
                # 等待所有任务完成
                for future in as_completed(futures):
                    if future.result():
                        keep_going = True
            except KeyboardInterrupt:
                # 捕获Ctrl+C退出
                print("\nProcess interrupted by user.")
                break

        # 处理已标记为删除的图像并移动
        for i, marked in enumerate(rm_img):
            if marked:
                source_path = image_paths[i]
                destination_path = os.path.join(output_dir, os.path.basename(source_path))
                if not os.path.exists(output_dir):
                    os.makedirs(output_dir)
                shutil.move(source_path, destination_path)
                print(f"\nMoved {os.path.basename(source_path)} to {output_dir}")

    print("\nProcessing completed.")

# 主函数
def main():
    try:
        parser = argparse.ArgumentParser(description="Move images with low similarity to another directory.")
        parser.add_argument('--input_dir', type=str, required=True, help='Path to the input image folder')
        parser.add_argument('--output_dir', type=str, required=True, help='Path to the output folder for similarity images')
        parser.add_argument('--threshold_ssim', type=float, default=0.9, help='Similarity threshold for image comparison (0 to 1)')
        parser.add_argument('--batch_size', type=int, default=10, help='Number of images to process in a batch')
        args = parser.parse_args()

        move_similar_images(args.input_dir, args.output_dir, args.threshold_ssim, args.batch_size)

    except KeyboardInterrupt:
        # 捕获 Ctrl+C 中断
        print("\nProgram interrupted by user. Exiting gracefully...")

if __name__ == "__main__":
    main()
