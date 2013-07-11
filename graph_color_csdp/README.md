Program for solving chromatic number problem (graph multicoloring). 
Implementing global search with constraints and branch and bound techniques.
Program uses Semidefinite programming relaxation to find the lower bound of chromatic number.
Computing the lower bound as Lovasz Number using CSDP solver library. 
Download and compile Csdp before linking this program.
Read more about CSDP at https://projects.coin-or.org/Csdp/

Lovasz theta value (number) of complementary graph gives very tight relaxation for chromatic number. But it computing itdepends on the connecting rate between nodes, not depending on color values. 
This research is stucked, need to find the way to compute the theta function value with constraints of already assigned colorvalues to some nodes.
