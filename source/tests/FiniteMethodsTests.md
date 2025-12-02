FiniteMethods – testes e validação
=================================

Tabela de conteúdos
- Histórico recente
- Como rodar
- Casos cobertos
- Referência de projeto

Como rodar
- Build dos testes: `cd projects/GenesysTerminalApplication && make CONF=Debug build-tests`
- Executar somente FiniteMethods: `./build/Debug/GNU-Linux/tests/TestFiles/f1 --gtest_filter=FiniteMethods.*`
- Resultado atual (última execução): 5/5 testes PASS.

Casos cobertos
- `DiffusionNoneKeepsState`: garante que o modo `NONE` não altera o campo.
- `ForwardEulerExplicitFEMStep`: passo explícito FEM lumped (malha 1D, fronteiras Dirichlet) produz incremento esperado.
- `BackwardEulerImplicitStep`: passo implícito (backward Euler) resolve sistema tridiagonal FEM e respeita fronteiras.
- `CrankNicolsonImplicitIsLessDiffusive`: semi-implícito (Crank–Nicolson) amortiza menos que backward Euler no mesmo passo.
- `BoundariesStayFixed`: confirma preservação das condições de contorno (nós de fronteira).

Referência de projeto
- Detalhes de engenharia, diagrama simplificado e limitações: `documentation/FiniteMethodsDesign.md`.
