'''
convert labelimg's xml to yolo's txt
usage:
    # python xml2txt.py -h
    python xml2txt.py --class_names car,person --input_folder /path/to/input_folder --output_folder /path/to/output_folder
'''
import os
import argparse
import xml.etree.ElementTree as ET

def convert_xml_to_yolo(class_names, xml_file, output_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()

    # 获取图像的宽度和高度
    image_width = int(root.find('size/width').text)
    image_height = int(root.find('size/height').text)

    with open(output_file, 'w') as out_file:
        for obj in root.iter('object'):
            class_name = obj.find('name').text

            # 确保类别名在类别列表中
            if class_name in class_names:
                class_id = class_names.index(class_name)  # 获取类别 ID
            else:
                # 如果类别不在列表中，跳过这个物体
                continue

            # 获取边界框信息
            bndbox = obj.find('bndbox')
            xmin = int(bndbox.find('xmin').text)
            ymin = int(bndbox.find('ymin').text)
            xmax = int(bndbox.find('xmax').text)
            ymax = int(bndbox.find('ymax').text)

            # 计算 YOLO 格式的边界框
            x_center = round((xmin + xmax) / 2.0 / image_width, 6)
            y_center = round((ymin + ymax) / 2.0 / image_height, 6)
            width = round((xmax - xmin) / float(image_width), 6)
            height = round((ymax - ymin) / float(image_height), 6)

            # 将结果写入文件，格式为：类别 ID，中心点 x，中心点 y，宽度，高度
            out_file.write(f"{class_id} {x_center} {y_center} {width} {height}\n")

def convert_folder_xml_to_yolo(class_names, input_folder, output_folder):
    for xml_file in os.listdir(input_folder):
        if xml_file.endswith('.xml'):
            xml_path = os.path.join(input_folder, xml_file)
            txt_file = xml_file.replace('.xml', '.txt')
            output_path = os.path.join(output_folder, txt_file)
            convert_xml_to_yolo(class_names, xml_path, output_path)
            print(f"Converted {xml_file} to {txt_file}")

def main():
    # 创建参数解析器
    parser = argparse.ArgumentParser(description="Convert labelImg XML to YOLO TXT format.")
    
    # 添加输入和输出路径的命令行参数
    parser.add_argument('--class_names', type=str, required=True, help='Comma-separated list of class names')
    parser.add_argument('--input_folder', type=str, required=True, help='Path to the input XML folder')
    parser.add_argument('--output_folder', type=str, required=True, help='Path to the output TXT folder')

    # 解析命令行参数
    args = parser.parse_args()

    # 将 class_names 参数分割成一个列表
    class_names = args.class_names.split(',')

    # 运行转换
    convert_folder_xml_to_yolo(class_names, args.input_folder, args.output_folder)

if __name__ == '__main__':
    main()


