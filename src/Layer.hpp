#ifndef LAYER_HPP_
#define LAYER_HPP_

class Layer {
public:
  virtual ~Layer() = default;

  virtual void OnUpdate() = 0;
};

#endif // LAYER_HPP_
