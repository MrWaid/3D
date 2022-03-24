#include "DirectTX.cpp""
#include <stdio.h>

const int LENGTH = 1000,
          WIDTH  = 600;

const int A = 4;

double Res[A][A] = {};
double Matrix_WH[WIDTH][LENGTH] = {};

double Znear = 19;

void Rectangle3D  (double  x_Cube, double y_Cube, double z_Cube, double z_C, double a_Cube,
                   double color1_R, double color1_G, double color1_B, double  line);
void Line_3D      (double  x1, double  y1, double  z1, double  color1_R, double  color1_G, double  color1_B,
                   double  x2, double  y2, double  z2, double  color2_R, double  color2_G, double  color2_B);
void Pixel_3D     (double  x, double  y, double  z, double  color_R, double  color_G, double  color_B);
void Triangle     (double  x1, double  y1, double  z1, double  color1_R, double  color1_G, double  color1_B,
                   double  x2, double  y2, double  z2, double  color2_R, double  color2_G, double  color2_B,
                   double  x3, double  y3, double  z3, double  color3_R, double  color3_G, double  color3_B);
double Lerp       (double  x1, double  x2, double  t);

int Matrix_Z (int x, int y, double z);
void Matrix  ();
void Change (double* x1, double* y1, double* z1, double* color1_R, double* color1_G, double* color1_B,
             double* x2, double* y2, double* z2, double* color2_R, double* color2_G, double* color2_B);

void ssLoadIdentity   ();
void ssTranslate      (double dx, double dy, double dz);
void ssScale          (double dx, double dy, double dz);
void ssRotateX        (double dx);
void ssRotateY        (double dy);
void ssRotateZ        (double dz);
void ArrayMultiply    (double a[A][A], double b[A][A], double c[A][A]);
void ArrayMultiply1x3 (double a[A][A], double b[A], double c[A]);
void Divide           (double a[A]);
void arr_copy         (double a[A][A], double b[A][A]);
void PrintMatrix      (double matrix[A][A]);
void Pause            ();

int main ()
{
    txTextCursor (false);
    DirectTXCreateWindow (LENGTH, WIDTH, "3D куб");

    ssLoadIdentity();

    double Res_Copy1[A][A] = {};
    double Res_Copy2[A][A] = {};
    double copy_res [A][A] = {};

    double X_Cube1 = 0, X_Cube2 = -4;
    double Y_Cube1 = 0, Y_Cube2 = -1;
    double Z_Cube1 = 20, Z_Cube2 = 19.9;
    double Z_C1    = 0.4, Z_C2    = 0.3;
    double A_Cube1 = 4, A_Cube2 = 2;
    double colorR_Cube1 = 255, colorR_Cube2 = 1;
    double colorG_Cube1 = 1, colorG_Cube2 = 255;
    double colorB_Cube1 = 1, colorB_Cube2 = 1;
    double Line_Cube1 = 1, Line_Cube2 = 1;

    assert (Line_Cube1 > 0 && Line_Cube2 > 0);

    int matrix_1 = 1, matrix_2 = 1;

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        arr_copy (copy_res, Res);

        Matrix ();

        DirectTXBegin();

        DirectTXSetFillColor (TX_BLACK);
        DirectTXDrawRectangle (0, 0, LENGTH, WIDTH);

        arr_copy (Res_Copy1, Res);

        Rectangle3D (X_Cube1, Y_Cube1, Z_Cube1, Z_C1, A_Cube1,
                     colorR_Cube1, colorG_Cube1, colorB_Cube1, Line_Cube1);

        DirectTXFlush();

        DirectTXEnd ();

        if (GetAsyncKeyState(VK_F1)) ssScale (GetAsyncKeyState(VK_SHIFT)? 1.01 : 0.99, 1, 1);
        if (GetAsyncKeyState(VK_F2)) ssScale (1, GetAsyncKeyState(VK_SHIFT)? 1.01 : 0.99, 1);
        if (GetAsyncKeyState(VK_F3)) ssScale (1, 1, GetAsyncKeyState(VK_SHIFT)? 1.01 : 0.99);

        if (GetAsyncKeyState(VK_F4)) ssRotateX (GetAsyncKeyState(VK_SHIFT)? 0.1 : -0.1);
        if (GetAsyncKeyState(VK_F5)) ssRotateY (GetAsyncKeyState(VK_SHIFT)? 0.1 : -0.1);
        if (GetAsyncKeyState(VK_F6)) ssRotateZ (GetAsyncKeyState(VK_SHIFT)? 0.1 : -0.1);

        if (GetAsyncKeyState(VK_F7))  ssTranslate (GetAsyncKeyState(VK_SHIFT)? 0.1 : -0.1, 0, 0);
        if (GetAsyncKeyState(VK_F8)) ssTranslate (0, GetAsyncKeyState(VK_SHIFT)? 0.1 : -0.1, 0);
        if (GetAsyncKeyState(VK_F9)) ssTranslate (0, 0, GetAsyncKeyState(VK_SHIFT)? 0.1 : -0.1);

        if (GetAsyncKeyState('Q')) matrix_1 = 0;
        if (GetAsyncKeyState('A')) matrix_1 = 1;
        if (GetAsyncKeyState('W')) matrix_2 = 0;
        if (GetAsyncKeyState('S')) matrix_2 = 1;

        if (GetAsyncKeyState(VK_RETURN)) ssLoadIdentity();

    }

    return 0;
}

void ssTranslate (double dx, double dy, double dz)
{
    double Translate[A][A] = {1, 0, 0, dx,
                              0, 1, 0, dy,
                              0, 0, 1, dz,
                              0, 0, 0, 1};
    double copy_res[A][A] = {};

    arr_copy (copy_res, Res);

    ArrayMultiply (Translate, copy_res, Res);
}

void ssScale (double dx, double dy, double dz)
{
    double Scale[A][A] = {dx, 0,  0,  0,
                          0,  dy, 0,  0,
                          0,  0,  dz, 0,
                          0,  0,  0,  1};

    double copy_res[A][A] = {};

    arr_copy (copy_res, Res);

    ArrayMultiply (Scale, copy_res, Res);
}

void ssRotateX (double dx)
{
    double Rotate[A][A] = {1,  0,        0,         0,
                           0,  cos (dx), -sin (dx), 0,
                           0,  sin (dx), cos (dx),  0,
                           0,  0,        0,        1};

    double copy_res[A][A] = {};

    arr_copy (copy_res, Res);

    ArrayMultiply (Rotate, copy_res, Res);
}

void ssRotateY (double dy)
{
    double Rotate[A][A] = {cos (dy) , 0,  sin (dy), 0,
                           0,         1,  0,        0,
                           -sin (dy), 0,  cos (dy), 0,
                           0,         0,  0,        1};

    double copy_res[A][A] = {};

    arr_copy (copy_res, Res);

    ArrayMultiply (Rotate, copy_res, Res);
}

void ssRotateZ (double dz)
{
    double Rotate[A][A] = {cos (dz), -sin (dz), 0, 0,
                           sin (dz),  cos (dz), 0, 0,
                           0,         0,        1, 0,
                           0,         0,        0, 1};

    double copy_res[A][A] = {};

    arr_copy (copy_res, Res);

    ArrayMultiply (Rotate, copy_res, Res);
}

void Line_3D (double  x1, double  y1, double  z1, double  color1_R, double  color1_G, double  color1_B,
              double  x2, double  y2, double  z2, double  color2_R, double  color2_G, double  color2_B)
{
    for (double t = 0; t <= 1; t += 0.002)
    {
        double x       = Lerp (x1, x2, t);
        double y       = Lerp (y1, y2, t);
        double z       = Lerp (z1, z2, t);
        double color_R = Lerp (color1_R, color2_R, t);
        double color_G = Lerp (color1_G, color2_G, t);
        double color_B = Lerp (color1_B, color2_B, t);

        Pixel_3D (x, y, z, color_R, color_G, color_B);
    }
}

void Rectangle3D (double x_Cube, double y_Cube, double z_Cube, double z_C, double a_Cube, double color1_R, double color1_G, double color1_B,
                  double line)
{
    double color2_R = color1_R / 2, color2_G = color1_G / 2  , color2_B = 1  ;
    double color3_R = color1_R, color3_G = color1_G  , color3_B = 1  ;
    double color4_R = color1_R / 2, color4_G = color1_G / 2  , color4_B = 1  ;
    double color5_R = color1_R / 2, color5_G = color1_G / 2  , color5_B = 1  ;
    double color6_R = color1_R / color1_R  , color6_G = color1_G / color1_G  , color6_B = 1  ;
    double color7_R = color1_R / 2, color7_G = color1_G / 2  , color7_B = 1  ;
    double color8_R = color1_R / color1_R  , color8_G = color1_G / color1_G  , color8_B = 1  ;

    Triangle (x_Cube         , y_Cube         , z_Cube + z_C, color5_R, color5_G, color5_B,
              x_Cube + a_Cube, y_Cube         , z_Cube + z_C, color6_R, color6_G, color6_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube + z_C, color8_R, color8_G, color8_B);//5-6-8
    Triangle (x_Cube         , y_Cube         , z_Cube + z_C, color5_R, color5_G, color5_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube + z_C, color8_R, color8_G, color8_B,
              x_Cube         , y_Cube - a_Cube, z_Cube + z_C, color7_R, color7_G, color7_B);//5-8-7

    Triangle (x_Cube + a_Cube, y_Cube - a_Cube, z_Cube      , color4_R, color4_G, color4_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube + z_C, color8_R, color8_G, color8_B,
              x_Cube         , y_Cube - a_Cube, z_Cube + z_C, color7_R, color7_G, color7_B);//4-8-7
    Triangle (x_Cube         , y_Cube - a_Cube, z_Cube      , color3_R, color3_G, color3_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube      , color4_R, color4_G, color4_B,
              x_Cube         , y_Cube - a_Cube, z_Cube + z_C, color7_R, color7_G, color7_B);//3-4-7

    Triangle (x_Cube         , y_Cube         , z_Cube      , color1_R, color1_G, color1_B,
              x_Cube         , y_Cube         , z_Cube + z_C, color5_R, color5_G, color5_B,
              x_Cube         , y_Cube - a_Cube, z_Cube + z_C, color7_R, color7_G, color7_B);//1-5-7
    Triangle (x_Cube         , y_Cube         , z_Cube      , color1_R, color1_G, color1_B,
              x_Cube         , y_Cube - a_Cube, z_Cube      , color3_R, color3_G, color3_B,
              x_Cube         , y_Cube - a_Cube, z_Cube + z_C, color7_R, color7_G, color7_B);//1-3-7

    Triangle (x_Cube         , y_Cube         , z_Cube      , color1_R, color1_G, color1_B,
              x_Cube         , y_Cube         , z_Cube + z_C, color5_R, color5_G, color5_B,
              x_Cube + a_Cube, y_Cube         , z_Cube + z_C, color6_R, color6_G, color6_B);//1-5-6
    Triangle (x_Cube         , y_Cube         , z_Cube      , color1_R, color1_G, color1_B,
              x_Cube + a_Cube, y_Cube         , z_Cube      , color2_R, color2_G, color2_B,
              x_Cube + a_Cube, y_Cube         , z_Cube + z_C, color6_R, color6_G, color6_B);//1-2-6

    Triangle (x_Cube + a_Cube, y_Cube         , z_Cube      , color2_R, color2_G, color2_B,
              x_Cube + a_Cube, y_Cube         , z_Cube + z_C, color6_R, color6_G, color6_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube + z_C, color8_R, color8_G, color8_B);//2-6-8
    Triangle (x_Cube + a_Cube, y_Cube         , z_Cube      , color2_R, color2_G, color2_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube      , color4_R, color4_G, color4_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube + z_C, color8_R, color8_G, color8_B);//2-4-8

    Triangle (x_Cube         , y_Cube         , z_Cube      , color1_R, color1_G, color1_B,
              x_Cube         , y_Cube - a_Cube, z_Cube      , color3_R, color3_G, color3_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube      , color4_R, color4_G, color4_B);//1-3-4
    Triangle (x_Cube         , y_Cube         , z_Cube      , color1_R, color1_G, color1_B,
              x_Cube + a_Cube, y_Cube         , z_Cube      , color2_R, color2_G, color2_B,
              x_Cube + a_Cube, y_Cube - a_Cube, z_Cube      , color4_R, color4_G, color4_B);//1-2-4
    //*/

    for (double y = y_Cube; y >= y_Cube - a_Cube; y -= a_Cube / line)
    {
        Line_3D (x_Cube         , y, z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube + a_Cube, y, z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-2, 3-4

        Line_3D (x_Cube         , y, z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube         , y, z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-5, 3-7

        Line_3D (x_Cube + a_Cube, y, z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube + a_Cube, y, z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //2-6, 4-8

        Line_3D (x_Cube         , y, z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube + a_Cube, y, z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //5-6, 7-8
    }

    for (double x = x_Cube; x <= x_Cube + a_Cube; x += a_Cube / line)
    {
        Line_3D (x, y_Cube         , z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x, y_Cube - a_Cube, z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-3, 2-4

        Line_3D (x, y_Cube         , z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x, y_Cube - a_Cube, z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //5-7, 6-8

        Line_3D (x, y_Cube         , z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x, y_Cube         , z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-5, 2-6

        Line_3D (x, y_Cube - a_Cube, z_Cube      , 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x, y_Cube - a_Cube, z_Cube + z_C, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-5, 2-6
    }

    for (double z = z_Cube; z <= z_Cube + z_C; z += z_C / line)
    {
        Line_3D (x_Cube         , y_Cube         , z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube         , y_Cube - a_Cube, z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-3, 5-7

        Line_3D (x_Cube + a_Cube, y_Cube         , z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube + a_Cube, y_Cube - a_Cube, z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //2-4, 6-8

        Line_3D (x_Cube         , y_Cube         , z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube + a_Cube, y_Cube         , z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-2, 5-6

        Line_3D (x_Cube         , y_Cube - a_Cube, z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128,
                 x_Cube + a_Cube, y_Cube - a_Cube, z, 127 + (color1_R / 2), 127 + (color1_G / 2), 128); //1-2, 5-6
    }
    //*/
}

void Pixel_3D (double x, double y, double z, double color_R, double color_G, double color_B)
{
    double xyz_copy[A] = {x, y, z, 1};
    double xyz     [A] = {};

    ArrayMultiply1x3 (Res, xyz_copy, xyz);

    Divide (xyz);

    x = LENGTH / 2 + xyz[0] * 50 / (xyz[2] - Znear);
    y = WIDTH  / 2 - xyz[1] * 50 / (xyz[2] - Znear);

            if (0 <= x && x <= LENGTH && 0 <= y && y <= WIDTH)
            {
                if (Matrix_Z (x, y, z) == 1)
                {
                    DirectTXPutPixel (x, y, RGB (color_R, color_G, color_B));
                }
            }
}

void Triangle (double x1, double y1, double z1, double color1_R, double color1_G, double color1_B,
               double x2, double y2, double z2, double color2_R, double color2_G, double color2_B,
               double x3, double y3, double z3, double color3_R, double color3_G, double color3_B)
{
    /*for (double t = 0; t <= 1; t += 0.002)
    {
        if (In (y2, y1, y3));
        else
        {
            if (In (y3, y1, y2)) Change (&x2, &y2, &z2, &color2_R, &color2_G, &color2_B,
                                         &x3, &y3, &z3, &color3_R, &color3_G, &color3_B);
            else
            {
                if (In (y1, y2, y3)) Change (&x2, &y2, &z2, &color2_R, &color2_G, &color2_B,
                                             &x1, &y1, &z1, &color1_R, &color1_G, &color1_B);
                else
                {
                    if (In (y3, y2, y1))
                    {
                        Change (&x2, &y2, &z2, &color2_R, &color2_G, &color2_B,
                                &x3, &y3, &z3, &color3_R, &color3_G, &color3_B);
                        Change (&x1, &y1, &z1, &color1_R, &color1_G, &color1_B,
                                &x3, &y3, &z3, &color3_R, &color3_G, &color3_B);
                    }
                    else
                    {
                        if (In (y1, y3, y2))
                        {
                            Change (&x2, &y2, &z2, &color2_R, &color2_G, &color2_B,
                                    &x3, &y3, &z3, &color3_R, &color3_G, &color3_B);
                            Change (&x2, &y2, &z2, &color2_R, &color2_G, &color2_B,
                                    &x1, &y1, &z1, &color1_R, &color1_G, &color1_B);
                        }
                        else
                        {
                            if (In (y2, y3, y1)) Change (&x1, &y1, &z1, &color1_R, &color1_G, &color1_B,
                                                         &x3, &y3, &z3, &color3_R, &color3_G, &color3_B);
                        }
                    }
                }
            }
        }

        double x4       = Lerp (x1      , x2      , t);
        double y4       = Lerp (y1      , y2      , t);
        double z4       = Lerp (z1      , z2      , t);
        double color4_R = Lerp (color1_R, color2_R, t);
        double color4_G = Lerp (color1_G, color2_R, t);
        double color4_B = Lerp (color1_B, color2_B, t);

        double x5       = Lerp (x3      , x2      , t);
        double y5       = Lerp (y3      , y2      , t);
        double z5       = Lerp (z3      , z2      , t);
        double color5_R = Lerp (color3_R, color2_R, t);
        double color5_G = Lerp (color3_G, color2_R, t);
        double color5_B = Lerp (color3_B, color2_B, t);

        Line_3D (x4, y4, z4, color4_R, color4_G, color4_B, x5, y5, z5, color5_R, color5_G, color5_B);
    }
    //*/
   /* for (double t = 0; t <= 1; t += 0.002)
    {
        if (y2 == y3)
        {
            double y12     = Lerp (y1, y2, t);
            double x12     = Lerp (x1, x2, t);
            double x13     = Lerp (x1, x3, t);
            double z12     = Lerp (z1, z2, t);
            double z13     = Lerp (z1, z3, t);
            double color12_R = Lerp (color1_R, color2_R, t);
            double color12_G = Lerp (color1_G, color2_G, t);
            double color12_B = Lerp (color1_B, color2_B, t);
            double color13_R = Lerp (color1_R, color3_R, t);
            double color13_G = Lerp (color1_G, color3_G, t);
            double color13_B = Lerp (color1_B, color3_B, t);

            Line_3D (x13, y12, z13, color13_R, color13_G, color13_B, x12, y12, z12, color12_R, color12_G, color12_B);
        }

        if (y1 == y2)
        {
            double y31     = Lerp (y3, y1, t);
            double x31     = Lerp (x3, x1, t);
            double x32     = Lerp (x3, x2, t);
            double z31     = Lerp (z3, z1, t);
            double z32     = Lerp (z3, z2, t);
            double color31_R = Lerp (color3_R, color1_R, t);
            double color31_G = Lerp (color3_G, color1_G, t);
            double color31_B = Lerp (color3_B, color1_B, t);
            double color32_R = Lerp (color3_R, color2_R, t);
            double color32_G = Lerp (color3_G, color2_G, t);
            double color32_B = Lerp (color3_B, color2_B, t);

            Line_3D (x31, y31, z31, color31_R, color31_G, color31_B, x32, y31, z32, color32_R, color32_G, color32_B);
        }
    }
    //*/
}

void Change (double* x1, double* y1, double* z1, double* color1_R, double* color1_G, double* color1_B,
             double* x2, double* y2, double* z2, double* color2_R, double* color2_G, double* color2_B)
{
    double x = *x1, y = *y1, z = *z1, color_R = *color1_R, color_G = *color1_G, color_B = *color1_B;
    *x1 = *x2;
    *y1 = *y2;
    *z1 = *z2;
    *color1_R = *color2_R;
    *color1_G = *color2_G;
    *color1_B = *color2_B;
    *x2 = x;
    *y2 = y;
    *z2 = z;
    *color2_R = color_R;
    *color2_G = color_G;
    *color2_B = color_B;
}

int Matrix_Z (int x, int y, double z)
{
    assert (0 <= x && x <= LENGTH);
    assert (0 <= y && y <= WIDTH);

    if (Matrix_WH[y][x] >= z)
    {
        Matrix_WH[y][x] = z;
        return 1;
    }
    else return 0;
}

void Matrix ()
{
    for (int i = 0; i < WIDTH; i ++)
    {
        for (int j = 0; j < LENGTH; j ++)
        {
            Matrix_WH[i][j] = 100;
        }
    }
}

double Lerp (double x1, double x2, double t)
{
    x1 = x1 + t * (x2 - x1);

    return x1;
}

void ArrayMultiply (double a[A][A], double b[A][A], double c[A][A])
{
    for (int y = 0; y < A; y ++)
    {
        assert (0 <= y && y < A);

        for (int x = 0; x < A; x ++)
        {
            assert (0 <= x && x < A);

            c[y][x] = 0;

            for (int i = 0; i < A; i ++)
            {
                assert (0 <= i && i < A);

                c[y][x] += a[y][i] * b[i][x];
            }
        }
    }
}

void arr_copy (double a[A][A], double b[A][A])
{
    for (int y = 0; y < A; y ++)
    {
        assert (0 <= y && y < A);

        for (int x = 0; x < A; x ++)
        {
            assert (0 <= x && x < A);

            a[y][x] = b[y][x];
        }
    }
}

void ssLoadIdentity ()
{
    for (int y = 0; y < A; y ++)
    {
        assert (0 <= y && y < A);

        for (int x = 0; x < A; x ++)
        {
            assert (0 <= x && x < A);

            Res[y][x] = 0;
        }
    }

    for (int i = 0; i < A; i ++) Res[i][i] = 1;
}

void ArrayMultiply1x3 (double a[A][A], double b[A], double c[A])
{
    for (int y = 0; y < A; y ++)
    {
        assert (0 <= y && y < A);

        c[y] = 0;

        for (int x = 0; x < A; x ++)
        {
            assert (0 <= x && x < A);

            c[y] += a[y][x] * b[x];

        }
    }
}

void Divide (double a[A])
{
    for (int x = 0; x < A; x ++)
    {
        a[x] /= a[3];
    }
}

void PrintMatrix (double matrix[A][A])
{
    for (int y = 0; y < A; y ++)
    {
        assert (0 <= y && y < A);

        for (int x = 0; x < A; x ++)
        {
            assert (0 <= x && x < A);

            printf ("%+10.6lg ", matrix[y][x]);
        }
        printf ("\n");
    }
}

void Pause ()
{
    while (kbhit()) getch();
    getch();
}
