import torch
import cv2
import os
from pathlib import Path
import numpy as np
from PIL import Image
from torchvision.ops import nms
import argparse

# 设置设备，GPU 优先
device = 'cuda' if torch.cuda.is_available() else 'cpu'

def parse_opt():
    parser = argparse.ArgumentParser()
    parser.add_argument('--test-dir', type=str, required=True, help='Path to test images directory')
    parser.add_argument('--output-dir', type=str, required=True, help='Path to save output images')
    parser.add_argument('--model-path', type=str, required=True, help='Path to the trained model (best.pt)')
    parser.add_argument('--conf-thres', type=float, default=0.75, help='Confidence threshold for predictions')  # 默认置信度阈值为 0.75
    opt = parser.parse_args()
    return opt

def process_img(test_dir, output_dir, model_path, conf_thres):
    # 加载训练好的 YOLO 模型
    model = torch.load(model_path, map_location=device)['model'].float()
    model.eval()

    # 创建输出目录，如果不存在
    Path(output_dir).mkdir(parents=True, exist_ok=True)

    # 遍历测试数据目录中的图像文件
    for image_name in os.listdir(test_dir):
        if image_name.endswith(('.jpg', '.png', '.jpeg')):  # 仅处理图像文件
            image_path = os.path.join(test_dir, image_name)
            img = Image.open(image_path)

            # 将图像调整为输入模型的大小
            img_resized = img.resize((352, 352))  # 根据训练时的图像尺寸调整大小
            img_array = np.array(img_resized)
            img_tensor = torch.from_numpy(img_array).float() / 255.0  # 归一化到 [0, 1]
            img_tensor = img_tensor.permute(2, 0, 1).unsqueeze(0).to(device)  # 转换为 (1, 3, 640, 640)

            # 推理
            with torch.no_grad():
                predictions = model(img_tensor)

            # 提取预测结果
            predictions = predictions[0]  # 获取第一张图像的预测结果
            iou_thres = 0.45  # IOU 阈值

            predictions = predictions[predictions[:, 4] > conf_thres]  # 置信度过滤
            boxes = predictions[:, :4]
            scores = predictions[:, 4] * predictions[:, 5]  # 置信度 * 类别置信度

            # 应用 NMS（非最大抑制）
            indices = nms(boxes, scores, iou_threshold=iou_thres)

            # 读取原图用于绘制框
            img_cv = cv2.imread(image_path)

            # 绘制检测框
            for idx in indices:
                box = boxes[idx].cpu().numpy()
                x1, y1, x2, y2 = box
                img_cv = cv2.rectangle(img_cv, (int(x1), int(y1)), (int(x2), int(y2)), (255, 0, 0), 2)
                cv2.putText(img_cv, f'Class: {int(predictions[idx, 5])} Conf: {scores[idx]:.2f}', 
                            (int(x1), int(y1)-10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)

            # 保存结果图像
            output_path = os.path.join(output_dir, image_name)
            cv2.imwrite(output_path, img_cv)  # 保存图像

            print(f'Processed: {image_name}')

if __name__ == '__main__':
    # 解析命令行参数
    opt = parse_opt()

    # 处理图像
    process_img(opt.test_dir, opt.output_dir, opt.model_path, opt.conf_thres)
