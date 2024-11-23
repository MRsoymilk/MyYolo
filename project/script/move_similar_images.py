import os
import shutil
from PIL import Image
import numpy as np
from skimage.metrics import structural_similarity as ssim
from threading import Lock
from concurrent.futures import ThreadPoolExecutor, as_completed
import argparse
import sys
import time

# 读取指定目录的图片
def read_images(input_dir):
    return [
        os.path.join(input_dir, f)
        for f in os.listdir(input_dir)
        if f.lower().endswith((".png", ".jpg", ".jpeg"))
    ]


# 将图像划分为指定批次大小的列表
def divide_into_batches(image_paths, batch_size):
    return [image_paths[i : i + batch_size] for i in range(0, len(image_paths), batch_size)]


# 计算两张图像的 SSIM
def calculate_ssim(image1, image2):
    image1_gray = np.array(image1.convert("L"))
    image2_gray = np.array(image2.convert("L"))
    score, _ = ssim(image1_gray, image2_gray, full=True)
    return score


# 多线程批次处理
def process_batch(batch, batch_index, image_paths, rm_img, batch_img, threshold_ssim, lock, thread_index, total_count):
    keep_going = False

    for i, img_path1 in enumerate(batch):
        img1_index = image_paths.index(img_path1)
        if rm_img[img1_index]:  # 如果图像已被标记，跳过
            continue

        with lock:
            batch_img[img1_index] = batch_index  # 设置批次编号

        for j, img_path2 in enumerate(batch[i + 1 :], start=i + 1):
            img2_index = image_paths.index(img_path2)
            if rm_img[img2_index]:  # 如果图像已被标记，跳过
                continue

            # 计算 SSIM
            with Image.open(img_path1) as image1, Image.open(img_path2) as image2:
                similarity = calculate_ssim(image1, image2)

            # 输出当前进度到控制台
            sys.stdout.write(
                f"\033[{thread_index % 10 + 2}H T{thread_index + 1} - {total_count}/{len(batch)}:{i}/{j} "
                f"Comparing {os.path.basename(img_path1)} and {os.path.basename(img_path2)} SSIM = {similarity:.4f}\033[K\n"
            )
            sys.stdout.flush()

            if similarity < threshold_ssim:
                with lock:
                    rm_img[img2_index] = True  # 标记为需要删除
                keep_going = True

    return keep_going


# 主函数
def move_similar_images(input_dir, output_dir, threshold_ssim, batch_size):
    # 读取图像路径
    image_paths = read_images(input_dir)
    rm_img = [False] * len(image_paths)  # 标记删除
    batch_img = [0] * len(image_paths)  # 批次编号
    lock = Lock()  # 锁对象

    divided_img = divide_into_batches(image_paths, batch_size)
    keep_going = True
    epoch = 0

    while keep_going:
        keep_going = False
        epoch += 1
        print(f"\033[1H Epoch {epoch}: Processing batches...\n")
        sys.stdout.flush()

        with ThreadPoolExecutor() as executor:
            futures = [
                executor.submit(
                    process_batch,
                    batch,
                    batch_index + 1,
                    image_paths,
                    rm_img,
                    batch_img,
                    threshold_ssim,
                    lock,
                    thread_index,
                    len(image_paths),
                )
                for thread_index, (batch_index, batch) in enumerate(enumerate(divided_img))
            ]

            for future in as_completed(futures):
                keep_going = keep_going or future.result()

    # 移动已标记为删除的图像
    print("Start moving similar images...")
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for i, marked in enumerate(rm_img):
        if marked:
            source_path = image_paths[i]
            destination_path = os.path.join(output_dir, os.path.basename(source_path))
            shutil.move(source_path, destination_path)
            print(f"Moved {os.path.basename(source_path)} to {output_dir}")

    print("Processing completed.")


# 命令行入口
def main():
    parser = argparse.ArgumentParser(description="Move similar images to a specified directory.")
    parser.add_argument("--input_dir", type=str, required=True, help="Path to the input directory containing images.")
    parser.add_argument("--output_dir", type=str, required=True, help="Path to the output directory for similar images.")
    parser.add_argument("--threshold_ssim", type=float, default=0.9, help="Threshold for SSIM similarity (default: 0.9).")
    parser.add_argument("--batch_size", type=int, default=10, help="Number of images to process in each batch (default: 10).")
    args = parser.parse_args()

    move_similar_images(args.input_dir, args.output_dir, args.threshold_ssim, args.batch_size)


if __name__ == "__main__":
    tick = time.time()
    main()
    tock = time.time()
    elapsed_time = tock - tick
    hours = int(elapsed_time // 3600)
    minutes = int((elapsed_time % 3600) // 60)
    seconds = elapsed_time % 60
    print(f"Elapsed time: {hours} hours, {minutes} minutes, {seconds:.6f} seconds")
