int animateInt(int value, int target, int step) {
  int offset = target > value ? step : -1 * step;
  bool cond = offset == step ? value + offset < target : value + offset > target;
  if (cond)
    return value + offset;
  else
    return target;
}

float animateFloat(float value, float target, float step) {
  int offset = target > value ? step : -1 * step;
  bool cond = offset == step ? value + offset < target : value + offset > target;
  if (cond)
    return value + offset;
  else
    return target;
}