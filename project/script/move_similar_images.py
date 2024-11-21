import os
import argparse
import cv2
import shutil
from skimage.metrics import structural_similarity as ssim
from PIL import Image
import numpy as np

# 计算两张图像的SSIM相似度
def calculate_ssim(image1, image2):
    # 转为灰度图像
    image1_gray = cv2.cvtColor(image1, cv2.COLOR_BGR2GRAY)
    image2_gray = cv2.cvtColor(image2, cv2.COLOR_BGR2GRAY)
    
    # 计算SSIM相似度
    score, _ = ssim(image1_gray, image2_gray, full=True)
    return score

# 遍历目录中的图像文件，比较相似度并将相似度较低的图片移动到指定目录
def move_similar_images(input_folder, output_folder, similarity_threshold=0.9):
    # 获取目录中所有图像文件
    image_files = [f for f in os.listdir(input_folder) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]
    
    images = []
    for file in image_files:
        # 确保路径是 Unicode 格式，并且没有反斜杠问题
        file_path = os.path.normpath(os.path.join(input_folder, file))
        
        # 使用PIL读取图像
        try:
            pil_image = Image.open(file_path)
        except Exception as e:
            print(f"Failed to load image {file_path}: {e}")
            continue
        
        # 转换为OpenCV格式 (BGR)
        image = np.array(pil_image)
        if len(image.shape) == 3:  # 如果图像是彩色的
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        
        images.append((file, image))
    
    to_move = set()

    # 比较图像相似度
    len_imgs = len(images)
    for i in range(len_imgs):
        file_base, img_base = images[i]
        print(f"Handle {file_base}")
        if(file_base in to_move):
            continue
        for j in range(i + 1, len_imgs):
            file_cmp, img_cmp = images[j]
            similarity = calculate_ssim(img_base, img_cmp)
            print(f"|__ ({i + 1}/{len_imgs}/{j + 1}){file_cmp} SSIM = {similarity:.4f}")
            if similarity > similarity_threshold:
                print(f"|____ move {file_cmp}")
                to_move.add(file_cmp)
                
    # 创建输出目录，如果不存在
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    # 移动相似的图片
    for file in to_move:
        source_path = os.path.join(input_folder, file)
        destination_path = os.path.join(output_folder, file)
        shutil.move(source_path, destination_path)
        print(f"Moved {file} to {output_folder}")

def main():
    # 创建参数解析器
    parser = argparse.ArgumentParser(description="Move images with low similarity to another directory.")
    
    # 添加输入、输出目录和相似度阈值的命令行参数
    parser.add_argument('--input_folder', type=str, required=True, help='Path to the input image folder')
    parser.add_argument('--output_folder', type=str, required=True, help='Path to the output folder for similarity images')
    parser.add_argument('--similarity_threshold', type=float, default=0.9, help='Similarity threshold for image comparison (0 to 1)')

    # 解析命令行参数
    args = parser.parse_args()

    # 运行移动相似图像的操作
    move_similar_images(args.input_folder, args.output_folder, args.similarity_threshold)

if __name__ == "__main__":
    main()
