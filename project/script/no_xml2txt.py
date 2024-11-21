import os
import argparse

def convert_folder_no_xml_to_txt(input_folder, output_folder):
    # 确保输出文件夹存在
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    # 获取所有的图像文件（jpg, png, jpeg等）
    supported_image_formats = ('.jpg', '.jpeg', '.png')
    
    for filename in os.listdir(input_folder):
        # 判断文件是否为支持的图像格式
        if filename.lower().endswith(supported_image_formats):
            # 获取对应的xml文件名
            xml_filename = os.path.splitext(filename)[0] + '.xml'
            xml_file_path = os.path.join(input_folder, xml_filename)

            # 如果没有对应的xml文件，创建空的txt文件
            if not os.path.exists(xml_file_path):
                txt_filename = os.path.splitext(filename)[0] + '.txt'
                txt_file_path = os.path.join(output_folder, txt_filename)
                
                # 创建一个空的txt文件
                with open(txt_file_path, 'w') as f:
                    pass  # 文件为空
                print(f"Created empty TXT file for {filename}")

def main():
    # 创建参数解析器
    parser = argparse.ArgumentParser(description="Convert labelImg XML to YOLO TXT format.")
    
    # 添加输入和输出路径的命令行参数
    parser.add_argument('--input_folder', type=str, required=True, help='Path to the input folder containing image and XML files')
    parser.add_argument('--output_folder', type=str, required=True, help='Path to the output folder for TXT files')

    # 解析命令行参数
    args = parser.parse_args()

    # 运行转换
    convert_folder_no_xml_to_txt(args.input_folder, args.output_folder)

if __name__ == '__main__':
    main()
