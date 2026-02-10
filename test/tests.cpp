#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>
#include "../include/myproject/vector_library.hpp"

const double EPSILON = 0.1;

TEST_CASE("Add", "[vector]")
{
  const auto vec1 = Vector3d(1, 1, 1);
  const auto vec2 = Vector3d(3, 2, 1);
  const auto res = vec1 + vec2;
  REQUIRE(res.x == 4);
  REQUIRE(res.y == 3);
  REQUIRE(res.z == 2);
}

TEST_CASE("Sub", "[vector]")
{
  const auto vec1 = Vector3d(1, 1, 1);
  const auto vec2 = Vector3d(3, 2, 1);
  const auto res = vec2 - vec1;
  REQUIRE(res.x == 2);
  REQUIRE(res.y == 1);
  REQUIRE(res.z == 0);
}

TEST_CASE("length", "[vector]")
{
  const auto vec = Vector3d(1, 1, 1);
  REQUIRE(std::fabs(vec.length() - std::numbers::sqrt3) < EPSILON);
}

TEST_CASE("Scale", "[vector]")
{
  const auto vec = Vector3d(1, 1, 1) * 2;
  REQUIRE(vec.x == 2);
  REQUIRE(vec.y == 2);
  REQUIRE(vec.z == 2);
}

TEST_CASE("Dot", "[vector]")
{
  const auto dot = Vector3d(1, 1, 1).dot(Vector3d(2, 2, 2));
  REQUIRE(dot == 6);
}

TEST_CASE("Cross parallel vectors", "[vector]")
{
  const auto vec = Vector3d(1, 1, 1) * Vector3d(2, 2, 2);
  REQUIRE(vec.x == 0);
  REQUIRE(vec.y == 0);
  REQUIRE(vec.z == 0);
}

TEST_CASE("Cross", "[vector]")
{
  const auto vec = Vector3d(1, 1, 1) * Vector3d(3, 4, 5);
  REQUIRE(vec.x == 1);
  REQUIRE(vec.y == 2);
  REQUIRE(vec.z == 1);
}

TEST_CASE("Normalize unchanged", "[vector]")
{
  const auto vec = Vector3d(1, 0, 0);
  const auto norm = vec.norm();
  REQUIRE(vec.x == norm.x);
  REQUIRE(vec.y == norm.y);
  REQUIRE(vec.z == norm.z);
}

TEST_CASE("Norm produces unit vector and preserves direction", "[vector]")
{
  const auto sourceVector = Vector3d(3, 4, 12);
  const auto normalized = sourceVector.norm();

  const double mag2 = (normalized.x * normalized.x) + (normalized.y * normalized.y) + (normalized.z * normalized.z);
  REQUIRE(std::fabs(mag2 - 1.0) < EPSILON);

  const auto crossProd = normalized * sourceVector; // cross product should be near zero if parallel
  const double crossMag2 = (crossProd.x * crossProd.x) + (crossProd.y * crossProd.y) + (crossProd.z * crossProd.z);
  REQUIRE(crossMag2 < (EPSILON * EPSILON));
}


