//#include <stdexcept>
//#undef eigen_assert
//#define eigen_assert(x) \
//    if (!(x)) { throw (std::runtime_error("eigen_assert(" #x ") has failed\n")); }
#include "Eigen/Core"
#include <memory>
#include <vector>
#include <set>
#include <random>

class JointKet;
class QuBit {
  public:
    // all qubits would live through whole simulation, and would be deleted when simulation finished
    static std::set<QuBit*> all;
    static std::default_random_engine rnd_gen;
    
    friend class    JointKet;
    std::shared_ptr<JointKet> joint_ket;
    int get_index();
    
    QuBit();
    void reset();
    bool measure();
    static void clean_all();
};

class JointKet {
  public:
    Eigen::VectorXcd ket; // dynamic length column-vector of std::complex<double>
    std::set<QuBit*> qubits;
    JointKet(std::set<QuBit*> qubits_, Eigen::VectorXcd ket_);
};

template<typename Container, typename Key>
typename Container::difference_type find_index(const Container &cont, const Key &key) {
    auto pos = cont.find(key);
    if (pos == cont.end()) {
        return -1;
    }
    return std::distance(cont.begin(), pos);
}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp=1)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
    // unless the result is subnormal
           || std::abs(x-y) < std::numeric_limits<T>::min();
}

// quop is dynamic size (square) matrix of std::complex<double>
void apply_quop(const Eigen::MatrixXcd &quop, std::vector<QuBit*> args);

extern const Eigen::Matrix2cd X_op;
extern const Eigen::Matrix2cd Y_op;
extern const Eigen::Matrix2cd Z_op;
extern const Eigen::Matrix2cd S_op;
extern const Eigen::Matrix2cd T_op;
       const Eigen::MatrixXcd c_op(const Eigen::MatrixXcd &op, int n_c = 1);
extern const Eigen::Matrix2cd H_op;
extern const Eigen::Matrix4cd Swap_op;