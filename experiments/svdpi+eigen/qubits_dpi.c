#include "svdpi.h"
#include "Vtest__Dpi.h"
#include "qubits.h"

#ifdef __cplusplus
extern "C" {
#endif
    void* new_qubit() {
        return (void*) new QuBit();
    }
    #define SV2Q(sv,q) \
        QuBit* q = (QuBit* q) sv; \
        assert(QuBit::all.contains(q));
    
    int reset_qubit(void*  q_sv) {
        SV2Q(q_sv,q);
        q->reset();
        return 1;
    }
    
    int X   (void* q_sv) { SV2Q(q_sv,q); apply_quop(X_op,    {q}); return 1; }
    int Y   (void* q_sv) { SV2Q(q_sv,q); apply_quop(Y_op,    {q}); return 1; }
    int Z   (void* q_sv) { SV2Q(q_sv,q); apply_quop(Z_op,    {q}); return 1; }
    int S   (void* q_sv) { SV2Q(q_sv,q); apply_quop(S_op,    {q}); return 1; }
    int T   (void* q_sv) { SV2Q(q_sv,q); apply_quop(T_op,    {q}); return 1; }
    int H   (void* q_sv) { SV2Q(q_sv,q); apply_quop(H_op,    {q}); return 1; }
    int Swap(void* q_sv) { SV2Q(q_sv,q); apply_quop(Swap_op, {q}); return 1; }
    
    int cX   (void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(X_op),    {c, t}); return 1; }
    int cY   (void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(Y_op),    {c, t}); return 1; }
    int cZ   (void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(Z_op),    {c, t}); return 1; }
    int cS   (void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(S_op),    {c, t}); return 1; }
    int cT   (void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(T_op),    {c, t}); return 1; }
    int cH   (void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(H_op),    {c, t}); return 1; }
    int cSwap(void* c_sv, void* t_sv) { SV2Q(c_sv,c); SV2Q(t_sv,t); apply_quop(c_op(Swap_op), {c, t}); return 1; }
    
    int ccX   (void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(X_op, 2),    {c1, c0, t}); return 1; }
    int ccY   (void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(Y_op, 2),    {c1, c0, t}); return 1; }
    int ccZ   (void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(Z_op, 2),    {c1, c0, t}); return 1; }
    int ccS   (void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(S_op, 2),    {c1, c0, t}); return 1; }
    int ccT   (void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(T_op, 2),    {c1, c0, t}); return 1; }
    int ccH   (void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(H_op, 2),    {c1, c0, t}); return 1; }
    int ccSwap(void* c1_sv, void* c0_sv, void* t_sv) { SV2Q(c0_sv,c0); SV2Q(c1_sv,c1); SV2Q(t_sv,t); apply_quop(c_op(Swap_op, 2), {c1, c0, t}); return 1; }
    
    
#ifdef __cplusplus
}
#endif
