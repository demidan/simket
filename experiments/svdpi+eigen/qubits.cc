#include "qubits.h"
#include <iterator>
#include <map>
#include "unsupported/Eigen/KroneckerProduct"
//#include "unsupported/Eigen/MatrixFunctions"
#include <iostream>

using namespace Eigen;

// class QuBit
std::set<QuBit*> QuBit::all = {};

void QuBit::clean_all() {
    for(auto q : all) {
        delete q;
    }
    all.clear();
}

std::default_random_engine QuBit::rnd_gen = std::default_random_engine();

QuBit::QuBit() {
    reset();
    all.insert(this);
}
void QuBit::reset() {
    //joint_ket = std::make_shared<JointKet>({this});
    JointKet({this}, VectorXcd::Unit(2, 0));
}

int QuBit::get_index() {
    int index = find_index(this->joint_ket->qubits, this);
    assert(index != -1);
    return index;
}

bool QuBit::measure() {
    VectorXcd &ket = this->joint_ket->ket;
    int index = this->get_index();
    int backward_index = this->joint_ket->qubits.size() - index - 1;
    MatrixXcd ketbra = ket * ket.adjoint();
    
    MatrixXcd proj0 = (Matrix2cd() << 1., 0., 0., 0.).finished();
    MatrixXcd proj1 = (Matrix2cd() << 0., 0., 0., 1.).finished();
    proj0 = kroneckerProduct(MatrixXcd::Identity(1<<index, 1<<index), proj0);
    proj0 = kroneckerProduct(proj0, MatrixXcd::Identity(1<<backward_index, 1<<backward_index));
    proj1 = kroneckerProduct(MatrixXcd::Identity(1<<index, 1<<index), proj1);
    proj1 = kroneckerProduct(proj1, MatrixXcd::Identity(1<<backward_index, 1<<backward_index));
    
    std::complex<double> prob0 = (proj0 * ketbra).trace();
    std::complex<double> prob1 = (proj1 * ketbra).trace();
    assert(almost_equal(prob0.imag(), 0.) && prob0.real() >= 0. && prob0.real() <= 1.);
    assert(almost_equal(prob1.imag(), 0.) && prob0.real() >= 0. && prob0.real() <= 1.);
    assert(almost_equal(prob0.real() + prob1.real(), 1.));
    std::discrete_distribution<bool> distr = {prob0.real(), prob1.real()};
    
    bool measurement = distr(QuBit::rnd_gen);
    if (measurement) {
        ket = proj0 * ket;
    } else {
        ket = proj1 * ket;
    }
    ket.normalize();
    
    return measurement;
}

// class JointKet
JointKet::JointKet(std::set<QuBit*> qubits_, VectorXcd ket_) :
    ket(ket_),
    qubits(qubits_)
{
    assert(qubits.size() >= 1);
    for(auto q : qubits) {
        q->joint_ket = std::make_shared<JointKet>(*this);
    }
}

int bits_perm(int i, std::vector<int> bit_indices) {
    int j = 0;
    int mask = 1;
    int touched = 0;
    for (int ind : bit_indices) {
        if (i & 1<<ind)
            j |= mask;
        touched |= 1<<ind;
        mask <<= 1;
    }
    while (i & ~touched) {
        if (~touched & 1) {
            if (i & 1)
                j |= mask;
            mask <<= 1;
        }
        i       >>= 1;
        touched >>= 1;
    }
    return j;
}
PermutationMatrix<Dynamic> ket_perm(std::vector<int> qubit_indices, int size) {
    assert(size >= 1<<qubit_indices.size());
    
    //std::cout << "Construct bit permutation ";
    //for (auto i : qubit_indices) { std::cout << i << ' '; }
    //std::cout << " over " << size << " qubits\n";
    
    //ArrayXi indices = ArrayXi(1<<qubit_indices.size());
    VectorXi indices = VectorXi(size);
    for (int i = 0; i < size; ++i) {
        indices[i] = bits_perm(i, qubit_indices);
        //std::cout << "\tindex "<< i << " -> "<< indices[i]<<"\n";
    }
    return PermutationMatrix<Dynamic>(indices);
}

// useful identity: $(A \otimes B) (x \otimes y) = (A x) \otimes (B y)$,
//  where $\times$ -- is Kronecker product (~ tensor product), $A x$ -- matrix multiplication
void apply_quop(const MatrixXcd &quop, std::vector<QuBit*> args) {
    assert(quop.rows() == quop.cols());
    assert(quop.rows() == 1<<args.size());
    
    auto tangle = std::set<QuBit*>();
    auto kets   = std::set<JointKet*>();
    for (auto a : args) {
        auto k = a->joint_ket;
        //tangle.merge(k->qubits);
        tangle.insert(begin(k->qubits), end(k->qubits));
        kets.insert(k.get());
    }
    
    //VectorXcd new_ket = VectorXcd::Unit(1,1);
    MatrixXcd new_ket = MatrixXcd::Identity(1,1);
    int new_ket_size = 1<<tangle.size();
    for (auto k : kets) {
        new_ket = kroneckerProduct(k->ket, new_ket).eval();
    }
    assert(new_ket.size() == new_ket_size);
    
    int base_i = 0;
    auto ket_bases = std::map<JointKet*, int>();
    for (auto k : kets) {
        ket_bases[k] = base_i;
        base_i += k->ket.size();
    }
    std::vector<int> arg_indices = {};
    for (auto a : args) {
        arg_indices.push_back(ket_bases[a->joint_ket.get()] + find_index(a->joint_ket->qubits, a));
    }
    PermutationMatrix<Dynamic> perm = ket_perm(arg_indices, new_ket_size);
    
    int id_size = 1<<(tangle.size() - args.size());
    new_ket = perm.transpose() * (kroneckerProduct(MatrixXcd::Identity(id_size,id_size), quop) * (perm * new_ket));
    JointKet(tangle, new_ket);
}

const Matrix2cd X_op((Matrix2cd() <<
    0.,  1.,
    1.,  0.
).finished());
const Matrix2cd Y_op((Matrix2cd() <<
     0.,     std::complex<double>(0., -1.),
    std::complex<double>(0., 1.),  0.
).finished());
const Matrix2cd Z_op((Matrix2cd() <<
    1.,  0.,
    0., -1.
).finished());
const Matrix2cd S_op((Matrix2cd() <<
    1.,  0.,
    0., std::complex<double>(0., 1.)
).finished());
const Matrix2cd T_op((Matrix2cd() <<
    1.,  0.,
    0., std::complex<double>(1., 1.)/std::sqrt(2)
).finished());

const MatrixXcd c_op(const MatrixXcd &op, int n_c) {
    assert(n_c >= 1.);
    MatrixXcd controlled = MatrixXcd();
    controlled << 
        MatrixXcd::Identity(1<<n_c, 1<<n_c), MatrixXcd::Zero(1<<n_c, op.cols()),
        MatrixXcd::Zero(op.rows(),  1<<n_c), op;
    return controlled;
}

const Matrix2cd H_op(std::sqrt(2)/2 * (Matrix2cd() <<
    1.,  1.,
    1., -1.
).finished());
const Matrix4cd Swap_op((Matrix4cd() <<
    1.,  0.,  0.,  0.,
    0.,  0.,  1.,  0.,
    0.,  1.,  0.,  0.,
    0.,  0.,  0.,  1.
).finished());
