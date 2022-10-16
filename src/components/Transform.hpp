#ifndef SRC_COMPONENTS_TRANSFORM_HPP_
#define SRC_COMPONENTS_TRANSFORM_HPP_

class Transform final {
 public:
  Transform(const double x, const double y) noexcept;

  Transform(Transform&&) noexcept;

  ~Transform() noexcept = default;

  Transform& operator=(Transform&&) noexcept;

 public:
  double x;

  double y;
};

#endif  // SRC_COMPONENTS_TRANSFORM_HPP_