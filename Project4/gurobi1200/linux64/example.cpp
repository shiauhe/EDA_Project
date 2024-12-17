#include "gurobi_c++.h"
#include <iostream>

int main() {
    try {
        // 創建環境
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "gurobi.log");
        env.start();

        // 創建模型
        GRBModel model = GRBModel(env);

        // 添加變數
        GRBVar x = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x");
        GRBVar y = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "y");

        // 設置目標函數
        model.setObjective(3 * x + 2 * y, GRB_MAXIMIZE);

        // 添加約束條件
        model.addConstr(x + y <= 4, "c1");
        model.addConstr(x - y >= 1, "c2");

        // 優化模型
        model.optimize();

        // 輸出結果
        if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            std::cout << "Optimal objective: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
            std::cout << "x: " << x.get(GRB_DoubleAttr_X) << std::endl;
            std::cout << "y: " << y.get(GRB_DoubleAttr_X) << std::endl;
        } else {
            std::cout << "No optimal solution found." << std::endl;
        }

    } catch (GRBException& e) {
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << e.getMessage() << std::endl;
    } catch (...) {
        std::cerr << "Exception during optimization." << std::endl;
    }
    return 0;
}
