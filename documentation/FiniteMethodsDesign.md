FiniteMethods – visão de engenharia
==================================

Objetivo
- Simular difusão 1D com FEM linear (P1), coeficiente constante, malha uniforme, contornos Dirichlet, passos temporais explícitos, backward Euler ou Crank–Nicolson.

Decisões de engenharia e desempenho
- Malha uniforme 1D: a primeira dimensão da `Variable` define o número de nós; simplifica montagem e garante custo O(N).
- Massa lumped: diagonaliza a matriz de massa, permitindo passo explícito O(N) e sistema tridiagonal leve para os modos implícitos.
- Sistema tridiagonal: solver Thomas (`_solveTridiagonal`) com custo O(N) e baixo uso de memória.
- Contornos Dirichlet: nós de fronteira preservados; contribuição incorporada no RHS para passos implícitos.
- Estabilidade: modo explícito requer `alpha*dt/h^2` pequeno (p.ex. <= 0.5); modos implícitos são incondicionalmente estáveis.
- Persistência/controle: propriedades expostas via `SimulationControlGeneric*` para integração com o kernel (field/time variables, alpha, h, dt, discretização/solver, arquivo de saída).

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
