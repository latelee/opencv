/**
利用ssd检测物体
运行示例：
./a.out
代码默认已经设置模型文件和图片了。
输出detections.jpg图片。有红框。

注：VGG_VOC0712_SSD_300x300_iter_120000.caffemodel要自己训练出来，或者从网上下载一个。
*/

#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

const size_t width = 300;
const size_t height = 300;

static Mat getMean(const size_t& imageHeight, const size_t& imageWidth)
{
    Mat mean;

    const int meanValues[3] = {104, 117, 123};
    vector<Mat> meanChannels;
    for(int i = 0; i < 3; i++)
    {
        Mat channel((int)imageHeight, (int)imageWidth, CV_32F, Scalar(meanValues[i]));
        meanChannels.push_back(channel);
    }
    cv::merge(meanChannels, mean);
    return mean;
}

static Mat preprocess(const Mat& frame)
{
    Mat preprocessed;
    frame.convertTo(preprocessed, CV_32F);
    resize(preprocessed, preprocessed, Size(width, height)); //SSD accepts 300x300 RGB-images

    Mat mean = getMean(width, height);
    cv::subtract(preprocessed, mean, preprocessed);

    return preprocessed;
}

const char* about = "This sample uses Single-Shot Detector "
                    "(https://arxiv.org/abs/1512.02325)"
                    "to detect objects on image\n"; // TODO: link

// 不懂怎么传参数进去。。。
const char* params
    = "{ help           | false | print usage         }"
      "{ proto          | deploy.prototxt      | model configuration }"
      "{ model          | VGG_VOC0712_SSD_300x300_iter_120000.caffemodel      | model weights       }"
      "{ image          | hello.jpg       | image for detection }"
      "{ min_confidence | 0.5   | min confidence      }";

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, params);

    if (parser.get<bool>("help"))
    {
        std::cout << about << std::endl;
        parser.printMessage();
        return 0;
    }

    String modelConfiguration = parser.get<string>("proto");
    String modelBinary = parser.get<string>("model");

    //! [Create the importer of Caffe model]
    Ptr<dnn::Importer> importer;

    // Import Caffe SSD model
    try
    {
        importer = dnn::createCaffeImporter(modelConfiguration, modelBinary);
    }
    catch (const cv::Exception &err) //Importer can throw errors, we will catch them
    {
        cerr << err.msg << endl;
    }
    //! [Create the importer of Caffe model]

    if (!importer)
    {
        cerr << "Can't load network by using the following files: " << endl;
        cerr << "prototxt:   " << modelConfiguration << endl;
        cerr << "caffemodel: " << modelBinary << endl;
        cerr << "Models can be downloaded here:" << endl;
        cerr << "https://github.com/weiliu89/caffe/tree/ssd#models" << endl;
        exit(-1);
    }

    //! [Initialize network]
    dnn::Net net;
    importer->populateNet(net);
    importer.release();          //We don't need importer anymore
    //! [Initialize network]

    cv::Mat frame = cv::imread(parser.get<string>("image"), -1);

    if (frame.channels() == 4)
        cvtColor(frame, frame, COLOR_BGRA2BGR);
    //! [Prepare blob]
    Mat preprocessedFrame = preprocess(frame);

    Mat inputBlob = blobFromImage(preprocessedFrame); //Convert Mat to batch of images
    //! [Prepare blob]

    //! [Set input blob]
    net.setInput(inputBlob, "data");                //set the network input
    //! [Set input blob]

    cv::TickMeter t;
    
    t.start();
    
    //! [Make forward pass]
    Mat detection = net.forward("detection_out");                                  //compute output
    //! [Make forward pass]
    
    t.stop();
    
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    float confidenceThreshold = parser.get<float>("min_confidence");
    for(int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if(confidence > confidenceThreshold)
        {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

            float xLeftBottom = detectionMat.at<float>(i, 3) * frame.cols;
            float yLeftBottom = detectionMat.at<float>(i, 4) * frame.rows;
            float xRightTop = detectionMat.at<float>(i, 5) * frame.cols;
            float yRightTop = detectionMat.at<float>(i, 6) * frame.rows;

            std::cout << "Class: " << objectClass << std::endl;
            std::cout << "Confidence: " << confidence << std::endl;

            std::cout << " " << xLeftBottom
                      << " " << yLeftBottom
                      << " " << xRightTop
                      << " " << yRightTop << std::endl;

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                        (int)(xRightTop - xLeftBottom),
                        (int)(yRightTop - yLeftBottom));

            rectangle(frame, object, Scalar(0, 0, 255), 2);
        }
    }

    //imshow("detections", frame);
    //waitKey();

    std::cout << "Time: " << (double)t.getTimeMilli() / t.getCounter() << " ms (average from " << t.getCounter() << " iterations)" << std::endl;
    
    imwrite("detections.jpg", frame);

    return 0;
} // main
