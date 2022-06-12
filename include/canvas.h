#pragma once
#include <SDL.h>

#include <cmath>
#include <vector>

using namespace std;

void render_circle(SDL_Renderer* renderer, double x, double y, double r);

class Figure
{
public:
    /* Canvasに描く図形についてのクラス */
    void draw(SDL_Renderer* renderer, double center_x, double center_y,
              double scale) const;
};

class Circle : public Figure
{
    /* Canvasで扱う円のクラス */
private:
    double x;  // x座標
    double y;  // y座標
    double r;  // 半径
public:
    Circle(double x, double y, double r);
    void draw(SDL_Renderer* renderer, double center_x, double center_y,
              double scale) const;
};

class Line : public Figure
{
    /* Canvasで扱う直線のクラス */
private:
    // 直線の端点
    double x1;
    double y1;
    double x2;
    double y2;

public:
    Line(double x1, double y1, double x2, double y2);
    void draw(SDL_Renderer* renderer, double center_x, double center_y,
              double scale) const;
};

class Canvas
{
    /* Figureを移動・拡大縮小して表示するクラス */
private:
    vector<Figure> figures;
    int width;
    int height;

public:
    Canvas(int width, int height);
    void clear();
    void add_figure(Figure figure);
    void draw(SDL_Renderer* renderer, double center_x, double center_y,
              double scale) const;
};