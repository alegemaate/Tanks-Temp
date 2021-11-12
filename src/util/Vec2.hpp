#ifndef SRC_UTIL_VEC2_H_
#define SRC_UTIL_VEC2_H_

namespace util {

template <typename T>
class Vec2 {
 public:
  Vec2() : x(0), y(0);

  Vec2(T x, T y) : x(x), y(y);

  Vec2(const Vec2<T>& vector) : x(vector.x), y(vector.y);

  template <typename T>
  Vec2<T> operator-(const Vec2<T>& right) {
    return Vec2<T>(-right.x, -right.y);
  }

  template <typename T>
  Vec2<T>& operator+=(Vec2<T>& left, const Vec2<T>& right) {
    left.x += right.x;
    left.y += right.y;

    return left;
  }

  template <typename T>
  Vec2<T>& operator-=(Vec2<T>& left, const Vec2<T>& right) {
    left.x -= right.x;
    left.y -= right.y;

    return left;
  }

  template <typename T>
  Vec2<T> operator+(const Vec2<T>& left, const Vec2<T>& right) {
    return Vec2<T>(left.x + right.x, left.y + right.y);
  }

  template <typename T>
  Vec2<T> operator-(const Vec2<T>& left, const Vec2<T>& right) {
    return Vec2<T>(left.x - right.x, left.y - right.y);
  }

  template <typename T>
  Vec2<T> operator*(const Vec2<T>& left, T right) {
    return Vec2<T>(left.x * right.x, left.y * right.y);
  }

  template <typename T>
  Vec2<T> operator*(T left, const Vec2<T>& right) {
    return Vec2<T>(left * right.x, left * right.y);
  }

  template <typename T>
  Vec2<T>& operator*=(Vec2<T>& left, T right) {
    left.x *= right;
    left.y *= right;
    return left;
  }

  template <typename T>
  Vec2<T> operator/(const Vec2<T>& left, T right) {
    return Vec2<T>(left.x / right, left.y / right);
  }

  template <typename T>
  Vec2<T>& operator/=(Vec2<T>& left, T right) {
    left.x /= right;
    left.y /= right;
    return left;
  }

  template <typename T>
  bool operator==(const Vec2<T>& left, const Vec2<T>& right) {
    return left.x == right.x && left.y == right.y;
  }

  template <typename T>
  bool operator!=(const Vec2<T>& left, const Vec2<T>& right) {
    return left.x != right.x || left.y != right.y;
  }

  T x = 0;
  T y = 0;
};

}  // namespace util

#endif  // SRC_UTIL_VEC2_H_
