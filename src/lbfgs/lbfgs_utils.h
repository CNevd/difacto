#ifndef _LBFGS_UTILS_H_
#define _LBFGS_UTILS_H_
#include "dmlc/memory_io.h"
namespace difacto {
namespace lbfgs {

struct Job {
  static const int kPrepareData = 1;
  static const int kInitServer = 2;
  static const int kInitWorker = 3;
  static const int kPushGradient = 4;
  static const int kPrepareCalcDirection = 5;
  static const int kCalcDirection = 6;
  static const int kLinearSearch = 7;
  static const int kSaveModel = 8;

  int type;
  std::vector<real_t> value;

  void SerializeToString(std::string* str) const {
    dmlc::MemoryStringStream ss(str);
    ss.Write(type);
    ss.Write(value);
  }
  void ParseFromString(const std::string& str) {
    auto pstr = str;
    dmlc::MemoryStringStream ss(&pstr);
    ss.Read(&type);
    ss.Read(&value);
  }
};

struct JobRets {
  void SerializeToString(std::string* str) const {
    dmlc::MemoryStringStream ss(str);
    ss.Write(type);
    ss.Write(value);
  }
  void ParseFromString(const std::string& str) {
    auto pstr = str;
    dmlc::MemoryStringStream ss(&pstr);
    ss.Read(&type);
    ss.Read(&value);
  }
  void Add(const std::string& other) {
    JobRets o; o.ParseFromString(other);
    if (value.empty()) {
      value.resize(o.value.size());
    } else {
      CHECK_EQ(value.size(), o.value.size());
    }
    for (size_t i = 0; i < value.size(); ++i) {
      value[i] += o.value[i];
    }
  }
  std::vector<real_t> value;
};


/**
 * \brief return <a, b>
 */
double InnerProduct(const SArray<real_t>& a, const SArray<real_t>& b, int nthreads) {
  double res = 0;
  CHECK_EQ(a.size(), b.size());
  real_t const *ap = a.data();
  real_t const *bp = b.data();
#pragma omp parallel for reduction(+:res) num_threads(nthreads)
  for (size_t i = 0; i < a.size(); ++i) res += ap[i] * bp[i];
  return res;
}


}  // namespace lbfgs
}  // namespace difacto
#endif  // _LBFGS_UTILS_H_
