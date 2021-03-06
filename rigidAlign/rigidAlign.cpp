// rigidAlign.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include "Eigen/Core"
#include "stdafx.h"

using namespace Eigen;
int main() {
  // init

  Matrix4d transform;
  std::vector<Vector4d> sourcepoints, targetpoints;
  double a, b, c, x, y, z;

  Matrix4d tempT;
  tempT << 0, 0, -1, 1, 0, -1, 0, 1, -1, 0, 0, 1, 0, 0, 0,
      1;  // pi, pi/2, 0, 1,1,1

  for (int i = 0; i < 10; i++) {
    Vector4d tvec = Vector4d(rand() % 100, rand() % 100, rand() % 100, 1);
    sourcepoints.push_back(tvec);
    targetpoints.push_back(tvec * tempT);
  }

  const double EPSILON = 0.01;
  double errsum = 0;
  do {
    Matrix4d ga = transGradA(a, b, c, x, y, z);
    Matrix4d gb = transGradB(a, b, c, x, y, z);
    Matrix4d gc = transGradC(a, b, c, x, y, z);

	double grada, gradb, gradc;

    for (int i = 0; i < sourcepoints.size; i++) {
      // Compute error

      Vector4d transformed = sourcepoints[i] * transform;
      Vector4d tmp = transformed;
      tmp -= targetpoints[i];
      tmp = tmp.cwiseProduct(tmp);
      errsum += tmp.x + tmp.y + tmp.z;

    }
    printf("error sum: %f\n", errsum);

  } while (errsum > EPSILON);


  return 0;
}

Matrix4d getTransform(double a, double b, double c, double x, double y,
                      double z) {
  double ca = cos(a);
  double cb = cos(b);
  double cc = cos(c);
  double sa = sin(a);
  double sb = sin(b);
  double sc = sin(c);
  Matrix4d ret;
  ret << ca * cb, ca * sb * sc - sa * cc, ca * sb * cc + sa * sc, x, sa * cb,
      sa * sb * sc + ca * cc, sa * sb * cc - ca * sc, y, -sb, cb * sc, cb * cc,
      z, 0, 0, 0, 1;
  return ret;
}
Matrix4d transGradA(double a, double b, double c, double x, double y,
                    double z) {
  double ca = cos(a);
  double cb = cos(b);
  double cc = cos(c);
  double sa = sin(a);
  double sb = sin(b);
  double sc = sin(c);
  Matrix4d ret;
  ret << -cb * sa, -ca * cc - sa * sb * sc, ca * sc - cc * sa * sb, 0, ca * cb,
      ca * sb * sc - cc * sa, ca * cc * sb + sa * sc, 0, 0, 0, 0, 0, 0, 0, 0, 0;
  return ret;
}

Matrix4d transGradB(double a, double b, double c, double x, double y,
                    double z) {
  double ca = cos(a);
  double cb = cos(b);
  double cc = cos(c);
  double sa = sin(a);
  double sb = sin(b);
  double sc = sin(c);
  Matrix4d ret;
  ret << -ca * sb, ca * cb * sc, ca * cb * cc, 0, -sa * sb, cb * sa * sc,
      cb * cc * sa, 0, -cb, -sb * sc, -cc * sb, 0, 0, 0, 0, 0;
  return ret;
}
Matrix4d transGradC(double a, double b, double c, double x, double y,
                    double z) {
  double ca = cos(a);
  double cb = cos(b);
  double cc = cos(c);
  double sa = sin(a);
  double sb = sin(b);
  double sc = sin(c);
  Matrix4d ret;
  ret << 0, ca * cc * sb + sa * sc, cc * sa - ca * sb * sc, 0, 0,
      cc * sa * sb - ca * sc, -ca * cc - sa * sb * sc, 0, 0, cb * cc, -cb * sc,
      0, 0, 0, 0, 0;
  return ret;
}