#include <gauss_elimination.h>
#include <pendulum.h>

#include <cmath>
#include <vector>
using namespace std;

/*
振り子を表すクラス.
振り子の座標は, 左の方向にx軸, 下の方向にy軸が伸びている.
振り子の角度は, x軸の正の向きとの成す角.
*/
const double Pendulum::PI = 3.1415926535;
const double Pendulum::G = 4.0;
const double Pendulum::time_delta = 0.01;
Pendulum::Pendulum(int pendulum_num, double pendulum_string_length)
    : pendulum_num(pendulum_num), pendulum_string_length(pendulum_string_length)
{
    // 下に振り子が伸びるように初期化
    // pendulum_thetas = vector<double>(pendulum_num, PI / 2.0);
    pendulum_thetas = vector<double>(pendulum_num, 0.0);
    pendulum_velocitys = vector<double>(pendulum_num, 0.0);
    // for(int i = 0; i < pendulum_num; i++) {
    //     pendulum_thetas[i] = PI / 2.0 * (double)i;
    // }
}

vector<pair<double, double>> Pendulum::compute_coords() const
{
    /* 固定端を原点とする振り子の座標を返す. */
    vector<pair<double, double>> ret(pendulum_num);
    double now_x = 0.0, now_y = 0.0;  // 1つ上の振り子の座標
    for(int i = 0; i < pendulum_num; i++) {
        now_x = now_x + pendulum_string_length * cos(pendulum_thetas[i]);
        now_y = now_y + pendulum_string_length * sin(pendulum_thetas[i]);
        ret[i] = {now_x, now_y};
    }
    return ret;
}

void Pendulum::compute_A(double** A) const
{
    /* thetaの2回微分についての方程式の係数行列をAに格納する. */
    for(int i = 0; i < pendulum_num; i++) {
        for(int j = 0; j < pendulum_num; j++) {
            A[i][j] = (double)(pendulum_num - (double)max(i, j)) *
                      pendulum_string_length *
                      cos((PI / 2.0 - pendulum_thetas[i]) -
                          (PI / 2.0 - pendulum_thetas[j]));
        }
    }
    return;
}

void Pendulum::compute_b(double* b) const
{
    for(int i = 0; i < pendulum_num; i++) {
        b[i] = 0.0;
        for(int j = 0; j < pendulum_num; j++) {
            if(i == j) {
                b[i] = b[i] + (pendulum_num - i) * G *
                                  sin(PI / 2.0 - pendulum_thetas[i]);
            } else {
                b[i] = b[i] + (double)(pendulum_num - max(i, j)) *
                                  pendulum_string_length *
                                  pendulum_velocitys[j] *
                                  pendulum_velocitys[j] *
                                  sin((PI / 2.0 - pendulum_thetas[i]) -
                                      (PI / 2.0 - pendulum_thetas[j]));
            }
        }
    }
    return;
}

void Pendulum::move()
{
    double** A = (double**)malloc(sizeof(double*) * pendulum_num);
    double* b = (double*)malloc(sizeof(double) * pendulum_num);
    double* x = (double*)malloc(sizeof(double) * pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        A[i] = (double*)malloc(sizeof(double) * pendulum_num);
    }
    compute_A(A);
    compute_b(b);
    gauss_elimination(A, b, x, pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        pendulum_velocitys[i] = pendulum_velocitys[i] + x[i] * time_delta;
        pendulum_thetas[i] =
            pendulum_thetas[i] + pendulum_velocitys[i] * time_delta;
    }
}

int Pendulum::get_pendulum_num() const { return pendulum_num; }
