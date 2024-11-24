from dataclasses import dataclass, field
from typing import List
import time
import os
import shutil
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
from skimage.metrics import structural_similarity as ssim
from PIL import Image
import numpy as np



@dataclass
class IMG:
    id: int
    path: str
    gen: int
    batch: List[int] = field(default_factory=list) 
    rm: bool = False

def calculate_ssim(image1, image2):
    if image1.size != image2.size:
        return 1
    # 使用灰度图计算 SSIM
    image1_gray = np.array(image1.convert("L"))
    image2_gray = np.array(image2.convert("L"))
    score, _ = ssim(image1_gray, image2_gray, full=True)
    return score

def update_batch(img_array, batch_size, gen):
    remaining_images = [img for img in img_array if not img.rm]
    len_remain = len(remaining_images)
    count = 0
    batch_id = 0

    if gen == 0:
        for i in range(len_remain):
            remaining_images[i].gen = gen
            msg = 0
            remaining_images[i].batch.append(batch_id)
            count += 1
            if count >= batch_size:
                count = 0
                batch_id += 1
        return remaining_images

    new_arr = []
    is_handle = [False] * len_remain
    for i in range(len_remain):
        if is_handle[i]:
            continue
        copy_img = remaining_images[i]
        copy_img.gen = gen
        copy_img.batch.append(batch_id)
        is_handle[i] = True
        new_arr.append(copy_img)
        for j in range(i + 1, len_remain):
            if is_handle[j]:
                continue
            if batch_id not in remaining_images[j].batch:
                copy_img = remaining_images[j]
                copy_img.gen = gen
                copy_img.batch.append(batch_id)
                is_handle[j] = True
                new_arr.append(copy_img)
                count += 1
                if count >= batch_size - 1:
                    count = 0
                    batch_id += 1
                    break
    return new_arr

def process_batch(img_array: List[IMG], batch_index: int, threshold_ssim: float, thread_idx: int, epoch: int, total_count: int):
    keep_going = False
    batch_images = [img for img in img_array if img.batch[img.gen] == batch_index and not img.rm]
    for i, img1 in enumerate(batch_images):
        for img2 in batch_images[i + 1:]:
            with Image.open(img1.path) as image1, Image.open(img2.path) as image2:
                similarity = calculate_ssim(image1, image2)
            print(
                f"Epoch {epoch}: T{thread_idx + 1}/{total_count} "
                f"SSIM {img1.path} & {img2.path} = {similarity:.4f}"
            )
            if similarity < threshold_ssim:
                img2.rm = True
                keep_going = True

    return keep_going

def remove_img(img_array: List[IMG], output_dir: str):
    os.makedirs(output_dir, exist_ok=True)

    for img in img_array:
        if img.rm:
            source_path = img.path
            destination_path = os.path.join(output_dir, os.path.basename(source_path))
            shutil.move(source_path, destination_path)
            print(f"Moved {os.path.basename(source_path)} to {output_dir}")

def read_images(input_dir: str):
    return sorted(
        [
            os.path.join(input_dir, f)
            for f in os.listdir(input_dir)
            if f.lower().endswith((".png", ".jpg", ".jpeg"))
        ],
        key=lambda x: os.path.basename(x).lower()  # 按文件名排序（不区分大小写）
    )

def move_similar_images(input_dir, output_dir, threshold_ssim, batch_size):
    image_paths = read_images(input_dir)
    img_array = [IMG(id=i, path=path, gen=0, batch=[], rm=False) for i, path in enumerate(image_paths)]

    epoch = 0
    keep_going = True

    while keep_going:
        keep_going = False
        remove_img(img_array, output_dir)  # 移动被标记的图像
        img_array = update_batch(img_array, batch_size, gen=epoch)  # 更新批次
        batch_indices = range(max((img.batch[img.gen] for img in img_array if not img.rm), default=0) + 1)
        # mult thread ===========================================================
        with ThreadPoolExecutor() as executor:
            futures = [
                executor.submit(
                    process_batch,
                    img_array,
                    batch_index,
                    threshold_ssim,
                    thread_idx,
                    epoch,
                    len(img_array)
                )
                for thread_idx, batch_index in enumerate(batch_indices)
            ]
            for future in as_completed(futures):
                keep_going = keep_going or future.result()
        # single thread =========================================================
        # for thread_idx, batch_index in enumerate(batch_indices):
        #     res = process_batch(img_array, batch_index, threshold_ssim, thread_idx, len(img_array))
        #     keep_going = keep_going or res
        epoch += 1

# 执行测试
def main():
    parser = argparse.ArgumentParser(description="Move similar images to a specified directory.")
    parser.add_argument("--input_dir", type=str, required=True, help="Path to the input directory containing images.")
    parser.add_argument("--output_dir", type=str, required=True, help="Path to the output directory for similar images.")
    parser.add_argument("--threshold_ssim", type=float, default=0.9, help="Threshold for SSIM similarity (default: 0.9).")
    parser.add_argument("--batch_size", type=int, default=10, help="Number of images to process in each batch (default: 10).")
    args = parser.parse_args()

    move_similar_images(args.input_dir, args.output_dir, args.threshold_ssim, args.batch_size)
    # move_similar_images("/tmp/output", "/tmp/to_rm", 0.5, 10)

if __name__ == "__main__":
    tick = time.time()
    main()
    tock = time.time()
    elapsed_time = tock - tick
    hours = int(elapsed_time // 3600)
    minutes = int((elapsed_time % 3600) // 60)
    seconds = elapsed_time % 60
    print(f"Elapsed time: {hours} hours, {minutes} minutes, {seconds:.6f} seconds")
