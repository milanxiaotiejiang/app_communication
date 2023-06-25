//
// Created by Looper on 2022/10/8.
//

#include "exploration/tsp/genetic_TSP.h"
#include "exploration/tsp/nearest_neighbor_TSP.h"
#include "simulation.h"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

GeneticTSPSolver::GeneticTSPSolver(int min_number_of_gens, int const_generations)
        : abort_computation_(false) {
    min_number_of_generations_ = min_number_of_gens;
    const_generations_number_ = const_generations;
}

/*
     0   *1   *2   *3    4   *5   *6   *7   *8   *9  *10  *11  *12  *13   14  *15   16   17  *18  *19   20  *21  *22  *23  *24   25  *26  *27  *28  *29   30  *31  *32  *33  *34
 0   0  260  131  241  233  199  262  195  239   97  *89  318  172  190  104  255  303  123  276  136  324  292  186  135  131  147  304  288  314  205  163  156  197  236  223
 1 260    0  142   42   45   73  *17   64   51  162  173   95  145  117  285   79  107  273  114  286  141  137  157  257  329  336  139  142  157  177  352  271  243  193  249
 2 131  142    0  116  108  *75  144   84  131   39   56  206  123  122  156  146  198  144  185  157  233  201  147  147  200  207  213  199  225  173  223  175  178  205  195
 3 241  *42  116    0    8   41   44   45   56  143  154  120  143  115  266   84  126  254  125  267  161  148  141  248  310  317  159  153  177  174  333  268  241  193  225
 4 233   45  108   *8    0   33   48   41   58  135  146  124  139  111  258   86  129  246  128  259  165  151  151  240  302  309  163  156  180  185  325  260  237  201  243
 5 199   73   75   41   33    0   76  *31   74  102  112  144  122   94  225   98  144  212  137  225  174  161  134  206  269  275  172  165  190  168  291  227  220  187  226
 6 262   17  144   44   48   76    0   62  *43  165  175   78  143  115  288   69   91  275   97  288  125  120  143  255  332  338  132  125  149  173  354  269  233  187  242
 7 195   64   84   45  *41   31   62    0   47   98  109  122   97   69  221   68  116  209  106  222  145  129  101  202  265  272  141  134  158  135  288  223  195  157  201
 8 239   51  131   56   58   74   43   47    0  142  153   75  110   82  265  *28   69  243   64  256  100   87  101  222  299  306   98   92  116  130  322  236  192  144  196
 9  97  162  *39  143  135  102  165   98  142    0   17  221   92  110  122  158  212  107  185  121  234  202  116  108  166  173  213  199  226  142  189  134  140  174  164
10  89  173   56  154  146  112  175  109  153  *17    0  231   78   95  112  164  209   92  182  105  230  198  102   91  141  162  209  193  220  122  178  116  124  153  141
11 318   95  206  120  124  144   78  122   75  221  231    0  176 *148  330   72   38  295   68  307   67   80  154  288  345  371   85   91   99  182  387  301  243  176  242
12 172  145  123  143  139  122  143   97  110   92   78  176    0   34  171  103  157  149  130  162  179  147  *64  128  205  212  158  142  169   95  228  142  135  124  132
13 190  117  122  115  111   94  115   69   82  110   95  148  *34    0  189   75  129  166  102  179  151  119   54  146  223  229  130  114  141   88  245  160  133  113  134
14 104  285  156  266  258  225  288  221  265  122  112  330  171  189    0  257  302   38  275   42  323  291  183   81   51   66  303  287  295  205   82  106  182  235  208
15 255   79  146   84   86   98   69   68   28  158  164   72  103   75  257    0   52  223  *34  235   77   57   85  215  273  298   69   62   86  109  314  228  176  119  172
16 303  107  198  126  129  144   91  116   69  212  209  *38  157  129  302   52    0  272   36  282   34   41  133  256  333  339   46   53   58  150  355  270  228  142  211
17 123  273  144  254  246  212  275  209  243  107   92  295  149  166   38  223  272    0  243  *14  288  256  160   42   50   62  267  251  278  169   78   68  146  199  172
18 276  114  185  125  128  137   97  106   64  185  182   68  130  102  275   34   36  243    0  256   50  *20   98  222  299  306   33   28   52  115  322  236  193  108  176
19 136  286  157  267  259  225  288  222  256  121  105  307  162  179   42  235  282   14  256    0  297  265  169   46  *37   49  277  261  287  179   65   71  156  209  182
20 324  141  233  161  165  174  125  145  100  234  230   67  179  151  323   77  *34  288   50  297    0   41  137  264  341  347   36   49   31  148  363  278  223  135  205
21 292  137  201  148  151  161  120  129   87  202  198   80  147  119  291   57   41  256   20  265   41    0   99  232  309  315   14  *11   28  113  331  246  191  103  173
22 186  157  147  141  151  134  143  101  101  116  102  154   64   54  183   85  133  160   98  169  137   99    0  136  213  219  107   91  112  *34  235  150   91   60   91
23 135  257  147  248  240  206  255  202  222  108   91  288  128  146   81  215  256   42  222   46  264  232  136    0   89   96  243  227  254  145  112  *23  122  175  148
24 131  329  200  310  302  269  332  265  299  166  141  345  205  223   51  273  333   50  299   37  341  309  213   89    0  *16  320  304  331  222   32  114  199  252  225
25 147  336  207  317  309  275  338  272  306  173  162  371  212  229   66  298  339   62  306   49  347  315  219   96   16    0  327  311  337  229  *16  121  206  259  232
26 304  139  213  159  163  172  132  141   98  213  209   85  158  130  303   69   46  267   33  277   36   14  107  243  320  327    0   19  *17  121  343  257  198  110  180
27 288  142  199  153  156  165  125  134   92  199  193   91  142  114  287   62   53  251   28  261   49   11   91  227  304  311  *19    0   24  105  327  241  179   91  162
28 314  157  225  177  180  190  149  158  116  226  220   99  169  141  295   86   58  278   52  287  *31   28  112  254  331  337   17   24    0  121  353  268  193  105  175
29 205  177  173  174  185  168  173  135  130  142  122  182   95   88  205  109  150  169  115  179  148  113   34  145  222  229  121  105  121    0  245  159   77  *30   62
30 163  352  223  333  325  291  354  288  322  189  178  387  228  245  *82  314  355   78  322   65  363  331  235  112   32   16  343  327  353  245    0  137  222  275  248
31 156  271  175  268  260  227  269  223  236  134  116  301  142  160  106  228  270  *68  236   71  278  246  150   23  114  121  257  241  268  159  137    0  136  189  162
32 197  243  178  241  237  220  233  195  192  140  124  243  135  133  182  176  228  146  193  156  223  191   91 *122  199  206  198  179  193   77  222  136    0   94   30
33 236  193  205  193  201  187  187  157  144  174  153  176  124  113  235  119  142  199  108  209  135  103   60  175  252  259  110   91  105   30  275  189   94    0  *61
34 223  249  195  225  243  226  242  201  196  164  141  242  132  134  208  172  211  172  176  182  205  173   91  148  225  232  180  162  175   62  248  162  *30   61    0
 */

void GeneticTSPSolver::distance_matrix_thread(DistanceMatrix &distance_matrix_computation, cv::Mat &distance_matrix,
                                              const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                              double downsampling_factor,
                                              double robot_radius, double map_resolution, AStarPlanner &path_planner) {
    distance_matrix_computation.constructDistanceMatrix(distance_matrix, original_map, points, downsampling_factor,
                                                        robot_radius, map_resolution, pathplanner_);
}

void GeneticTSPSolver::abortComputation() {
    abort_computation_ = true;
}

double GeneticTSPSolver::getPathLength(const cv::Mat &path_length_Matrix, std::vector<int> given_path) {
    double length_of_given_path = 0;

    for (int i = 0; i < given_path.size() - 1; i++) {
        length_of_given_path += path_length_Matrix.at<double>(given_path[i], given_path[i + 1]);
    }

    return length_of_given_path;
}

// This Function takes the given path and mutates it. A mutation is a random change of the path-order. For example random
// nodes can be switched, or a random intervall of nodes can be inverted. Only the first and last Node can't be changed, because
// they are given from the Main-function.
std::vector<int> GeneticTSPSolver::mutatePath(const std::vector<int> &parent_path) {
    std::vector<int> mutated_path;

    std::vector<int> temporary_path;
    std::vector<int> saving_variable_path = parent_path;

    int what_to_change = (rand() % 2);

    if (what_to_change == 0) {
        int number_of_switches = (rand() % (parent_path.size() - 3)) + 1;
        for (int change = 0; change < number_of_switches; change++) {
            temporary_path.clear();
            bool switched = false;
            do {
                int node_one = (rand() % (saving_variable_path.size() - 2)) + 1;
                int node_two = (rand() % (saving_variable_path.size() - 2)) + 1;
                if (node_one != node_two) {
                    for (int node = 0; node < saving_variable_path.size(); node++) {
                        if (node == node_one) {
                            temporary_path.push_back(saving_variable_path[node_two]);
                        } else if (node == node_two) {
                            temporary_path.push_back(saving_variable_path[node_one]);
                        } else {
                            temporary_path.push_back(saving_variable_path[node]);
                        }
                    }
                    switched = true;
                }
            } while (!switched);
            saving_variable_path = temporary_path;
        }
        mutated_path = saving_variable_path;
    } else if (what_to_change == 1) {
        bool inverted = false;
        do {
            int node_one = (rand() % (saving_variable_path.size() - 2)) + 1;
            int node_two = (rand() % (saving_variable_path.size() - 2)) + 1;
            int inverting_counter = 0;
            if (node_one > node_two) {
                int tmp_node = node_one;
                node_one = node_two;
                node_two = tmp_node;
            }
            if (node_one != node_two) {
                for (int node = 0; node < parent_path.size(); node++) {
                    if (node < node_one || node > node_two) {
                        mutated_path.push_back(parent_path[node]);
                    } else {
                        mutated_path.push_back(parent_path[node_two - inverting_counter]);
                        inverting_counter++;
                    }
                }
                inverted = true;
            }
        } while (!inverted);
    } else {
        LOG_IF(INFO, DEBUG_EXPLORATION) << "Something was wrong in mutation-function.";
    }

    return mutated_path;
}

//This Function calculates the length of each given path and chooses the shortest one. It uses the getPathLength function.
std::vector<int>
GeneticTSPSolver::getBestPath(const std::vector<std::vector<int> > paths, const cv::Mat &pathlength_Matrix,
                              bool &changed) {
    std::vector<int> best_path = paths[0];

    double best_distance = getPathLength(pathlength_Matrix, paths[0]);

    for (int current_path = 1; current_path < paths.size(); current_path++) {
        double current_distance = getPathLength(pathlength_Matrix, paths[current_path]);
        if (current_distance < best_distance) {
            best_distance = current_distance;
            best_path = paths[current_path];
            changed = true;
        }
    }

    return best_path;
}

std::vector<int> GeneticTSPSolver::solveGeneticTSP(const cv::Mat &path_length_Matrix, const int start_Node) {
    std::vector<int> return_vector;
    NearestNeighborTSPSolver nearest_neighbor_solver;

    std::vector<int> calculated_path = nearest_neighbor_solver.solveNearestTSP(path_length_Matrix, start_Node);
    calculated_path.push_back(start_Node);

    if (path_length_Matrix.rows > 2) {
        bool changed_path = false;
        int changing_counter = const_generations_number_;

        int number_of_generations = 0;

        do {
            if (abort_computation_)
                return return_vector;

            number_of_generations++;
            changed_path = false;
            std::vector<std::vector<int> > current_generation_paths;
            current_generation_paths.push_back(calculated_path);
            for (int child = 0; child < 8; child++) {
                current_generation_paths.push_back(mutatePath(calculated_path));
            }
            calculated_path = getBestPath(current_generation_paths, path_length_Matrix, changed_path);
            if (number_of_generations >= min_number_of_generations_) {
                if (changed_path) {
                    changing_counter = const_generations_number_;
                } else {
                    changing_counter -= 1;
                }
            }
        } while (changing_counter > 0 || number_of_generations < min_number_of_generations_);
    }

    for (size_t node = 0; node < calculated_path.size() - 1; ++node) {
        return_vector.push_back(calculated_path[node]);
    }

    return return_vector;
}

/**
 *
 * @param original_map 经过预处理以及角度旋转后的地图数据，用来计算点跟点之间的路径长度
 * @param points 前面做区间分割的时候，计算得出的每一个区间的中心点坐标数组
 * @param downsampling_factor 地图的缩放比率，即把传入的地图按照给定的缩放比率进行缩放，在缩放后的地图上寻找路径
 * 需要注意的是，这种方式虽然用来处理大地图的时候，可以提高计算速度，但是存在一个隐患：当地图中存在窄通道时，缩放后该通道会闭合，从而导致两个区间不连通无法计算出区间距离
 * @param robot_radius 机器人的半径
 * @param map_resolution 栅格地图的分辨率
 * @param start_Node 初始区间序号，通过机器人当前坐标算出距离最近的区间
 * @param distance_matrix 用来返回距离矩阵
 * @return
 */
std::vector<int> GeneticTSPSolver::solveGeneticTSP(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                                   double downsampling_factor,
                                                   double robot_radius, double map_resolution, const int start_Node,
                                                   cv::Mat *distance_matrix) {
    LOG_IF(INFO, DEBUG_EXPLORATION) << "GeneticTSPSolver::solveGeneticTSP: Constructing distance matrix...";
    cv::Mat distance_matrix_ref;
    if (distance_matrix != nullptr)
        distance_matrix_ref = *distance_matrix;
    DistanceMatrix distance_matrix_computation;
    boost::thread
            t(
            boost::bind(&GeneticTSPSolver::distance_matrix_thread, this,
                        boost::ref(distance_matrix_computation),
                        boost::ref(distance_matrix_ref),
                        boost::cref(original_map),
                        boost::cref(points),
                        downsampling_factor, robot_radius, map_resolution,
                        boost::ref(pathplanner_)
            )
    );
    bool finished = false;
    while (!finished) {
        if (abort_computation_)
            distance_matrix_computation.abortComputation();
        finished = t.try_join_for(boost::chrono::milliseconds(10));
    }

    if (abort_computation_) {
        std::vector<int> return_vector;
        return return_vector;
    }

    return (solveGeneticTSP(distance_matrix_ref, start_Node));
}


std::vector<int>
GeneticTSPSolver::solveGeneticTSPClean(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                       double downsampling_factor, double robot_radius, double map_resolution,
                                       const int start_node) {
    cv::Mat distance_matrix_cleaned;
    std::map<int, int> cleaned_index_to_original_index_mapping;
    int new_start_node = start_node;
    DistanceMatrix distance_matrix_computation;
    distance_matrix_computation.computeCleanedDistanceMatrix(original_map, points, downsampling_factor, robot_radius,
                                                             map_resolution, pathplanner_,
                                                             distance_matrix_cleaned,
                                                             cleaned_index_to_original_index_mapping, new_start_node);

    return solveGeneticTSPWithCleanedDistanceMatrix(distance_matrix_cleaned, cleaned_index_to_original_index_mapping,
                                                    new_start_node);
}


std::vector<int> GeneticTSPSolver::solveGeneticTSPWithCleanedDistanceMatrix(const cv::Mat &distance_matrix,
                                                                            const std::map<int, int> &cleaned_index_to_original_index_mapping,
                                                                            const int start_node) {
    std::vector<int> optimal_order = solveGeneticTSP(distance_matrix, start_node);
    for (size_t i = 0; i < optimal_order.size(); ++i)
        optimal_order[i] = cleaned_index_to_original_index_mapping.at(optimal_order[i]);

    return optimal_order;
}

