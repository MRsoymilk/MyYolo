#ifndef CHECK_CV_ONNX_H
#define CHECK_CV_ONNX_H
#include <QString>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <unordered_map>
class Check_CV_Onnx
{
   public:
    Check_CV_Onnx();
    ~Check_CV_Onnx();
    bool loadModel(const QString &path, const bool &is_cpu = true);
    bool checkOnePicture(const QString &inputPath, const QString &outputPath);
    void setModelImgSize(const int &width, const int &height);
    void setConfidence(const float &threshold);
    void setNMS(const float &threshold);
    void setClasses(QStringList names);
    struct detect_info
    {
        float confidence_threshold = 0.5;
        float nms_threshold = 0.4;
        std::vector<int> class_ids;
        std::vector<float> confidences;
        std::vector<cv::Rect2d> boxes;
        std::vector<int> nms_indices;
        std::vector<std::string> class_names;
        int nc = 0;
        int model_width;
        int model_height;
    };
    std::vector<cv::Scalar> COLORS = {
        cv::Scalar(0, 255, 0),      // class 0 - Green
        cv::Scalar(255, 0, 0),      // class 1 - Blue
        cv::Scalar(0, 0, 255),      // class 2 - Red
        cv::Scalar(255, 0, 255),    // class 3 - Magenta
        cv::Scalar(255, 255, 0),    // class 4 - Cyan
        cv::Scalar(0, 128, 255),    // class 5 - Orange
        cv::Scalar(128, 0, 128),    // class 6 - Purple
        cv::Scalar(0, 255, 255),    // class 7 - Light Green
        cv::Scalar(255, 165, 0),    // class 8 - Orange Yellow
        cv::Scalar(255, 105, 180),  // class 9 - Hot Pink
        cv::Scalar(60, 179, 113),   // class 10 - Medium Sea Green
        cv::Scalar(255, 20, 147),   // class 11 - Deep Pink
        cv::Scalar(138, 43, 226),   // class 12 - Blue Violet
        cv::Scalar(75, 0, 130),     // class 13 - Indigo
        cv::Scalar(0, 191, 255),    // class 14 - Deep Sky Blue
        cv::Scalar(255, 69, 0)      // class 15 - Orange Red
    };

   private:
    void init();
    cv::Mat doDrawBox(const cv::Mat &img);
    bool doSaveImg(const cv::Mat &img, const QString &output_path);
    void doDataCheckout(const float *data);
    cv::Rect2d CoordinateTrans(const int &org_width, const int &org_height, const float &center_x,
                               const float &center_y, const float &width, float &height);
    void doDataCheckout(const float *data, const int &img_width, const int &img_height);
    void doRelease();

   private:
    detect_info m_info;
    cv::dnn::Net m_net;
};

#endif  // CHECK_CV_ONNX_H
