//
// Created by Looper on 2023/3/3.
//

#include "task/feedback.h"

void TaskFeedback::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this] {
            return !orderDeque.empty() || !dataDeque.empty();
        });

        if (!orderDeque.empty()) {
            if (orderDeque.back() == 0) {//end

            } else if (orderDeque.back() == 0) {//start

            }
            orderDeque.clear();
        } else if (!dataDeque.empty()) {
            _pose_type data = dataDeque.back();
            dataDeque.clear();

            LOG(INFO) << data.position.x << " " << data.position.y;
        }

    }
}
