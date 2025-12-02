FiniteMethods – testes e validação
=================================

Tabela de conteúdos
- Histórico recente
- Como rodar
- Casos cobertos
- Referência de projeto

Histórico desde "Test commiting compiling" (453b267a), excluindo-o
- 0b46543b “Faz o Genesys rodar”: baseline usado como ponto de partida.
- Inclusão do componente `FiniteMethods` (FEM 1D, massa lumped, solver tridiagonal) e plugin autoload.
- Smart de exemplo `Smart_FiniteMethods` criado e definido como app padrão (TraitsApp/TraitsTerminalApp); gera `temp/Smart_FiniteMethods.outputdatafile.txt` e `models/Smart_FiniteMethods.gen`.
- Makefiles Debug/Release ajustados para compilar `FiniteMethods`, o smart, testes e correção do alvo `DiffEquations_nomain`.
- Testes unitários `testFiniteMethods.cpp` cobrindo modos NONE, explícito, backward Euler, Crank–Nicolson e contornos; README de testes criado.
- Documentação de engenharia adicionada em `documentation/FiniteMethodsDesign.md` com decisões, uso e limitações.

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
