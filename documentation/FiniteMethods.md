FiniteMethods 
==================================

Componente 1.6: Componentes para modelagem e simulação de redes de componentes (nodos e transições em submodelos) - Componente para modelagem e simulação de elementos finitos.



Objetivo
- Simular difusão 1D com FEM linear (P1), coeficiente constante, malha uniforme, contornos Dirichlet, passos temporais explícitos, backward Euler ou Crank–Nicolson.

Decisões simplificadas
- Malha 1D uniforme: primeira dimensão de `Variable` é o número de nós; montagem e passos custam O(N).
- Massa lumped: matriz de massa fica diagonal; passo explícito O(N); modo implícito vira um sistema tridiagonal pequeno.
- Solver tridiagonal: Thomas (`_solveTridiagonal`) com O(N) e memória mínima.
- Contorno Dirichlet: nós de fronteira fixos; para modos implícitos, a fronteira entra no RHS.
- Estabilidade: explícito exige `alpha*dt/h^2 <= ~0.5`; implícitos são estáveis sem restrição.
- Controle/integração: `SimulationControlGeneric*` expõe variáveis de campo/tempo, alpha, h, dt, escolha de discretização/solver e arquivo de saída.

Desenho (simplificado)
```
Smart_FiniteMethods -> Create -> FiniteMethods -> Dispose
                          |           |
                          |           +-- Variable t (tempo)
                          +-- Variable u (campo 1D)
```

Instruções de build/execução
- Compilar app terminal: `cd projects/GenesysTerminalApplication && make CONF=Debug`
- Executar demo: `./dist/genesysterminalapplication` (gera `temp/Smart_FiniteMethods.outputdatafile.txt`)
- Rodar testes: `make CONF=Debug build-tests` e `./build/Debug/GNU-Linux/tests/TestFiles/f1 --gtest_filter=FiniteMethods.*`

Cobertura de testes (ver `source/tests/testFiniteMethods.cpp`)
- `DiffusionNoneKeepsState`: valida modo NONE.
- `ForwardEulerExplicitFEMStep`: passo explícito FEM lumped em malha 1D.
- `BackwardEulerImplicitStep`: passo implícito resolvendo sistema tridiagonal com contornos.
- `CrankNicolsonImplicitIsLessDiffusive`: semi-implícito com menor amortecimento.
- `BoundariesStayFixed`: garante preservação de Dirichlet.

Resultados atuais dos testes
- Comando: `./build/Debug/GNU-Linux/tests/TestFiles/f1 --gtest_filter=FiniteMethods.*`
- Resultado: 5/5 testes PASS.

Como usar o componente (ex.: `Smart_FiniteMethods.cpp`):
- Defina duas `Variable`: campo 1D (`u`) com dimensão N>=3 e tempos (`t`).
- Configure o componente chame `setFieldVariable`, `setTimeVariable`, `setDiffusionCoefficient` (alpha), `setSpaceStep` (h), `setTimeStep` (dt), `setDiscretization` (`FORWARD_EULER` ou `CRANK_NICOLSON`), `setSolver` (`EXPLICIT` ou `GAUSS_SEIDEL` para implícito/CN), `setFilename` opcional para gravação. Se usar um modelo `.gen`, esses valores podem ser editados no arquivo salvo.
- Conecte o componente no fluxo de entidades; `_onDispatchEvent` avança o campo até o tempo simulado e encaminha a entidade.

Limitações atuais
- Apenas 1D, coeficiente constante, contornos Dirichlet, malha uniforme. Não há materiais variáveis, fontes internas ou elementos de ordem superior.
