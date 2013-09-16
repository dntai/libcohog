#pragma once

#include <opencv2/opencv.hpp>
#include <libcohog.hpp>

namespace libcohog
{

// An element of ground truth
struct TruthRect
{
    cv::Rect rect;
    bool     confident;
};

// Describes the evaluation statistics of 1 or more frames
struct EvaluationResult
{
    // The total number of TPs, FPs, FNs with grouping of detection results
    int nTP, nFP, nFN;

    // The total number of FPs without grouping of detection results (for calculation of FPPW)
    int nFPW;

    // The total number of detection windows, and frames.
    long long int nWnd;
    int           nImg;

    EvaluationResult()
        :nTP(0), nFP(0), nFN(0), nFPW(0), nWnd(0), nImg(0)
    { }

    EvaluationResult operator+(const EvaluationResult& eval) const;
    EvaluationResult& operator+=(const EvaluationResult& eval);
    std::string to_string(bool one_line = true) const;
    double F_value() const;
    double FPPF() const;
    double FPPW() const;
    double Recall() const;
    double Precision() const;
    double Missrate() const;
    double FP_rate() const;
};


struct VerificationResult
{
    // The number of detection windows before thresholding
    int total_windows;

    // Detection windows thresholded without grouping
    std::vector<cv::Rect>   windows;
    std::vector<bool>       TP_flags;

    // Detection windows thresholded with grouping
    std::vector<cv::Rect>   windows_grouped;
    std::vector<bool>       TP_grouped_flags;

    // The rectangles of the ground truth
    std::vector<TruthRect>  ground_truth;
    std::vector<bool>       found_flags;

    EvaluationResult        to_eval() const;
};


std::vector<cv::Rect>   thresholding(const std::vector<Window>& windows, double th);
std::vector<cv::Rect>   grouping(const std::vector<cv::Rect>& rects, int groupTh, double eps);
cv::Rect                normalize_rectangle(const cv::Rect& r, double height_to_width_ratio, double height_ratio);
bool                    is_equivalent(const cv::Rect& r1, const cv::Rect& r2, double overwrap_th);

VerificationResult      verify(const DetectionResult& detection, const std::vector<TruthRect>& truth, 
                                    double threshold, double overwrap_th, int groupTh, double eps);

EvaluationResult        evaluate(const DetectionResult& detection, const std::vector<TruthRect>& truth, 
                            double threshold, double overwrap_th, int groupTh, double eps);
}

