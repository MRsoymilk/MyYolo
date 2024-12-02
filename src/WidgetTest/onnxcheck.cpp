#include "onnxcheck.h"

#include <QImage>

#include "../util/mydir.h"
#include "../util/mylog.h"

OnnxCheck::OnnxCheck() { init(); }

OnnxCheck::~OnnxCheck() {}

void OnnxCheck::init() {
  m_info.class_names = {"ZD6-A-BSG",  "ZD6-B-BSG",  "ZYJ7-A-BSG",
                        "ZYJ7-B-BSG", "ZYJ7-C-BSG", "ZYJ7-D-BSG"};
  m_info.nc = m_info.class_names.size();
  m_info.model_width = 352;
  m_info.model_height = 352;
  m_info.confidence_threshold = 0.5;
  m_info.nms_threshold = 0.4;
}

bool OnnxCheck::loadModel(const QString &path, const bool &is_cpu) {
  m_net = cv::dnn::readNetFromONNX(MyDir::Path2Path(path));
  if (m_net.empty()) {
    return false;
  }
  if (is_cpu) {
    m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
  } else {
    m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
  }
  return true;
}

bool OnnxCheck::checkOnePicture(const QString &inputPath,
                                const QString &outputPath) {
  cv::Mat img = cv::imread(MyDir::Path2Path(inputPath));
  if (img.empty()) {
    return false;
  }
  cv::Mat resized_img;
  cv::resize(img, resized_img,
             cv::Size(m_info.model_width, m_info.model_height));
  cv::Mat blob = cv::dnn::blobFromImage(
      resized_img, 1 / 255.0, cv::Size(m_info.model_width, m_info.model_height),
      cv::Scalar(), true, false);
  m_net.setInput(blob);
  std::vector<cv::Mat> outputs;
  m_net.forward(outputs, m_net.getUnconnectedOutLayersNames());

  for (const auto &output : outputs) {
    // int batch_size = output.size[0];
    int total_detections =
        output.size[1];  // num_anchors * grid_size * grid_size
    // int output_channels = output.size[2];  // classes + confidence + box

    // handle YOLOv5 output
    for (int i = 0; i < total_detections; ++i) {
      doDataCheckout(output.ptr<float>(0, i), img.cols, img.rows);
    }
  }

  if (m_info.boxes.empty() || m_info.confidences.empty()) {
    doSaveImg(img, outputPath);
    return false;
  }

  // NMS box
  cv::dnn::NMSBoxes(m_info.boxes, m_info.confidences,
                    m_info.confidence_threshold, m_info.nms_threshold,
                    m_info.nms_indices);

  cv::Mat out = doDrawBox(img);
  doSaveImg(out, outputPath);
  doRelease();
  return true;
}

void OnnxCheck::setModelImgSize(const int &width, const int &height) {
  if (width) {
    m_info.model_width = width;
  }
  if (height) {
    m_info.model_height = height;
  }
}

void OnnxCheck::setConfidence(const float &threshold) {
  m_info.confidence_threshold = threshold;
}

void OnnxCheck::setNMS(const float &threshold) {
  m_info.nms_threshold = threshold;
}

void OnnxCheck::setClasses(QStringList names) {
  m_info.nc = names.size();
  for (int i = 0; i < m_info.nc; ++i) {
    m_info.class_names.push_back(names.at(i).toStdString());
  }
}

cv::Rect2d OnnxCheck::CoordinateTrans(const int &org_width,
                                      const int &org_height,
                                      const float &center_x,
                                      const float &center_y, const float &width,
                                      float &height) {
  double x_factor = 1.0 * org_width / m_info.model_width;
  double y_factor = 1.0 * org_height / m_info.model_height;
  double x = (center_x - width / 2.0) * x_factor;
  double y = (center_y - height / 2.0) * y_factor;
  double w = width * x_factor;
  double h = height * y_factor;
  return cv::Rect2d{x, y, w, h};
}

void OnnxCheck::doDataCheckout(const float *data, const int &img_width,
                               const int &img_height) {
  float confidence = data[4];  // confidence
  if (confidence < m_info.confidence_threshold) {
    return;
  }

  // x, y, w, h -> index 0 to 3
  float x_center = data[0];  // box center x
  float y_center = data[1];  // box center y
  float width = data[2];     // box width
  float height = data[3];    // box height
  cv::Rect2d box =
      CoordinateTrans(img_width, img_height, x_center, y_center, width, height);
  // get class score -> index 5...
  int class_id = std::max_element(data + 5, data + 5 + m_info.nc) -
                 (data + 5);  // get nc count
  float class_confidence = data[5 + class_id];

  // info store
  m_info.boxes.push_back(box);
  m_info.confidences.push_back(confidence);
  m_info.class_ids.push_back(class_id);
}

bool OnnxCheck::doSaveImg(const cv::Mat &img, const QString &output_path) {
  cv::Mat img_rgb;
  cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);
  QImage qimg(img_rgb.data, img.cols, img.rows, img.step,
              QImage::Format_RGB888);
  return qimg.save(output_path);
}

void OnnxCheck::doRelease() {
  m_info.class_ids = {};
  m_info.confidences = {};
  m_info.boxes = {};
  m_info.nms_indices = {};
}

cv::Mat OnnxCheck::doDrawBox(const cv::Mat &img) {
  cv::Mat img_copy = img;
  for (int idx : m_info.nms_indices) {
    // draw bos
    cv::Rect box = m_info.boxes[idx];
    cv::rectangle(img_copy, box, COLORS.at(m_info.class_ids.at(idx)));
    // draw label: confidence { box info }
    std::string label = m_info.class_names.at(m_info.class_ids.at(idx));
    std::ostringstream oss_cfd;
    oss_cfd << std::fixed << std::setprecision(2)
            << m_info.confidences[idx] * 100 << "%";
    std::string confidence_str = oss_cfd.str();
    std::ostringstream oss_box;
    oss_box << std::fixed << std::setprecision(2) << "{" << box.x << ", "
            << box.y << ", " << box.width << ", " << box.height << "}";
    std::string box_info = oss_box.str();
    std::string info_base{label + ": " + confidence_str};
    int baseline = 0;
    cv::Size text_size =
        cv::getTextSize(info_base, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);
    cv::Point pos_base{box.x, box.y};
    pos_base.y = std::max(text_size.height, box.y - text_size.height);
    std::string info_box{box_info};
    cv::Point pos_box = {pos_base.x, pos_base.y + text_size.height + 1};
    cv::putText(img_copy, info_base, pos_base, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                COLORS.at(m_info.class_ids.at(idx)), 2);
    cv::putText(img_copy, info_box, pos_box, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                COLORS.at(m_info.class_ids.at(idx)), 2);
  }
  return img_copy;
}
