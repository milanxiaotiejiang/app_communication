//
// Created by Looper on 2023/6/6.
//

#include "exploration/tcr.h"
#include "vector"
#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"
#include "db/segmentation_data_base.h"


double tcr::coverageProportion() {
    try {
        auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
        double grid_spacing_in_meter = plan.robot_radius * std::sqrt(2);//网格正方形的边长
        double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution_from_subscription;
        LOG(INFO) << "grid size: " << grid_spacing_in_meter << " m   (" << grid_spacing_in_pixel << " px)";
        int spacing_half = (int) std::floor(0.5 * grid_spacing_in_pixel);

        const cv::Mat &baseMap = SegmentationCenter::instance().generateMat();

        std::vector<geometry_msgs::Pose2D> exploration_path;
        std::vector<cv::Point> point_path;
        std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;


        auto planMat = baseMap.clone();
        ExplorationCenter::instance().generatePlanningPathFull(planMat, ENERGY_FUNCTIONAL_EXPLORER_MODE, false,
                                                               exploration_path, point_path, complex_path);

        if (exploration_path.empty() || point_path.empty()) {
            return 0;
        }

        int area_px = 0;
        for (int v = 0; v < planMat.rows; ++v) {
            for (int u = 0; u < planMat.cols; ++u) {
                if (planMat.at<uchar>(v, u) >= 254)
                    area_px++;
            }
        }

        auto drawMat = baseMap.clone();

        for (int i = 1; i < point_path.size(); ++i) {
            cv::Point ps = cv::Point(point_path[i - 1].x, point_path[i - 1].y);
            cv::Point pe = cv::Point(point_path[i].x, point_path[i].y);
            cv::line(drawMat, ps, pe, cv::Scalar(200), spacing_half * 2);
        }

//    cv::imshow("plan_px", drawMat);
//    cv::waitKey();

        int plan_px = 0;
        for (int v = 0; v < drawMat.rows; ++v) {
            for (int u = 0; u < drawMat.cols; ++u) {
                if (drawMat.at<uchar>(v, u) == 200)
                    plan_px++;
            }
        }

        double proportion = (plan_px * 1.0 / area_px);
        LOG(INFO) << "### area_px : " << area_px
                  << " , plan_px = " << plan_px
                  << " , 规划面积/建图面积 = " << proportion;
        return proportion;
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }

    return 0;
}
