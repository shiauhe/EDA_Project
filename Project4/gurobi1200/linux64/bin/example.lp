Minimize
  latency

Subject To
  c1: start_h - start_a >= 1
  c2: start_h - start_c >= 1
  c3: start_j - start_d >= 1
  c4: start_i - start_c >= 1
  c5: start_l - start_h >= 1
  c6: start_l - start_i >= 1
  c7: start_m - start_i >= 1
  c8: start_m - start_j >= 1
  c9: start_n - start_l >= 1
  c10: start_n - start_m >= 1
  c11: start_o - start_b >= 1
  c12: start_o - start_h >= 1
  c13: start_o - start_k >= 1
  c14: start_p - start_g >= 1
  c15: start_q - start_n >= 1
  c16: latency - start_o >= 0
  c17: latency - start_p >= 0
  c18: latency - start_q >= 0

  r1: used_AND_t1 <= 2
  r2: used_OR_t1 <= 1
  r3: used_NOT_t1 <= 1
  r4: used_AND_t2 <= 2
  r5: used_OR_t2 <= 1
  r6: used_NOT_t2 <= 1

  r10: used_AND_t1 - 1 x_h_t1 - 2 x_i_t1 = 0
  r11: used_AND_t2 - x_l_t2 - x_m_t2 = 0
  r12: used_OR_t1 - x_j_t1 - x_k_t1 = 0
  r13: used_NOT_t3 - x_q_t3 = 0

Bounds
  start_h >= 0
  start_j >= 0
  start_i >= 0
  start_l >= 0
  start_m >= 0
  start_n >= 0
  start_o >= 0
  start_p >= 0
  start_q >= 0
  latency >= 0

Binary
  x_h_t1 x_i_t1 x_l_t2 x_m_t2 x_j_t1 x_k_t1 x_q_t3

End
