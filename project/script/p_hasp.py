import os
from PIL import Image
import imagehash


def read_images(input_dir):
    """
    读取目录中的所有图片路径
    :param input_dir: 输入图片目录
    :return: 图片路径列表
    """
    return [
        os.path.join(input_dir, f)
        for f in os.listdir(input_dir)
        if f.lower().endswith((".png", ".jpg", ".jpeg"))
    ]


def calculate_image_hash(image_path):
    """
    计算图片的感知哈希值
    :param image_path: 图片路径
    :return: 图片的感知哈希值
    """
    with Image.open(image_path) as img:
        return imagehash.phash(img)  # 使用感知哈希算法


def output_image_hashes(input_dir):
    """
    输出目录下每张图片的感知哈希值
    :param input_dir: 输入图片目录
    """
    image_paths = read_images(input_dir)
    if not image_paths:
        print(f"No images found in directory: {input_dir}")
        return

    print(f"Perceptual hashes of images in directory '{input_dir}':\n")
    for image_path in image_paths:
        img_hash = calculate_image_hash(image_path)
        print(f"{os.path.basename(image_path)}: {img_hash}")


if __name__ == "__main__":
    input_dir = "/home/vv/tmp/output_images/"  # 替换为你的图像目录路径
    output_image_hashes(input_dir)

