/*
 * FiniteMethods unit tests
 */

#include "../plugins/components/FiniteMethods.h"
#include "../../source/gtest/gtest/gtest.h"
#include "../kernel/simulator/Simulator.h"
#include <vector>

class FiniteMethodsAccess : public FiniteMethods {
public:
	using FiniteMethods::FiniteMethods;
	using FiniteMethods::_applyDiffusion;
};

class FiniteMethodsFixture {
public:
	FiniteMethodsFixture() : simulator(), model(simulator.getModelManager()->newModel()), finite(model) {}
	Simulator simulator;
	Model* model;
	FiniteMethodsAccess finite;
};

TEST(FiniteMethods, DiffusionNoneKeepsState) {
	FiniteMethodsFixture fx;
	fx.finite.setDiffusionModel(FiniteMethods::DiffusionModel::NONE);
	std::vector<double> current{1.0, 2.0, 3.0};
	std::vector<double> next;
	fx.finite._applyDiffusion(current, next);
	EXPECT_DOUBLE_EQ(next[0], 1.0);
	EXPECT_DOUBLE_EQ(next[1], 2.0);
	EXPECT_DOUBLE_EQ(next[2], 3.0);
}

TEST(FiniteMethods, ForwardEulerExplicitFEMStep) {
	FiniteMethodsFixture fx;
	fx.finite.setDiffusionModel(FiniteMethods::DiffusionModel::CONSTANT);
	fx.finite.setSolver(FiniteMethods::Solver::EXPLICIT);
	fx.finite.setDiscretization(FiniteMethods::Discretization::FORWARD_EULER);
	fx.finite.setDiffusionCoefficient(1.0);
	fx.finite.setSpaceStep(1.0);
	fx.finite.setTimeStep(0.1); // stable explicit step
	std::vector<double> current{1.0, 0.0, 0.0};
	std::vector<double> next;
	fx.finite._applyDiffusion(current, next);
	ASSERT_EQ(next.size(), current.size());
	EXPECT_DOUBLE_EQ(next.front(), 1.0);  // Dirichlet boundary preserved
	EXPECT_NEAR(next[1], 0.1, 1e-12);     // lumped mass explicit FEM: delta = 0.1
	EXPECT_DOUBLE_EQ(next.back(), 0.0);
}

TEST(FiniteMethods, BackwardEulerImplicitStep) {
	FiniteMethodsFixture fx;
	fx.finite.setDiffusionModel(FiniteMethods::DiffusionModel::CONSTANT);
	fx.finite.setSolver(FiniteMethods::Solver::GAUSS_SEIDEL);
	fx.finite.setDiscretization(FiniteMethods::Discretization::FORWARD_EULER); // interpreted as backward Euler when implicit
	fx.finite.setDiffusionCoefficient(1.0);
	fx.finite.setSpaceStep(1.0);
	fx.finite.setTimeStep(0.5);
	std::vector<double> current{1.0, 0.0, 0.0};
	std::vector<double> next;
	fx.finite._applyDiffusion(current, next);
	ASSERT_EQ(next.size(), current.size());
	EXPECT_DOUBLE_EQ(next.front(), 1.0);
	EXPECT_NEAR(next[1], 0.25, 1e-12); // solve (M + dt*K) u^{n+1} = M u^n
	EXPECT_DOUBLE_EQ(next.back(), 0.0);
}

TEST(FiniteMethods, CrankNicolsonImplicitIsLessDiffusive) {
	FiniteMethodsFixture fx;
	fx.finite.setDiffusionModel(FiniteMethods::DiffusionModel::CONSTANT);
	fx.finite.setSolver(FiniteMethods::Solver::GAUSS_SEIDEL);
	fx.finite.setDiscretization(FiniteMethods::Discretization::CRANK_NICOLSON);
	fx.finite.setDiffusionCoefficient(1.0);
	fx.finite.setSpaceStep(1.0);
	fx.finite.setTimeStep(0.5);
	std::vector<double> current{1.0, 0.0, 0.0};
	std::vector<double> next;
	fx.finite._applyDiffusion(current, next);
	ASSERT_EQ(next.size(), current.size());
	EXPECT_DOUBLE_EQ(next.front(), 1.0);
	EXPECT_NEAR(next[1], 1.0 / 3.0, 1e-12); // CN gives milder damping than backward Euler
	EXPECT_DOUBLE_EQ(next.back(), 0.0);
}

TEST(FiniteMethods, BoundariesStayFixed) {
	FiniteMethodsFixture fx;
	fx.finite.setDiffusionModel(FiniteMethods::DiffusionModel::CONSTANT);
	fx.finite.setSolver(FiniteMethods::Solver::GAUSS_SEIDEL);
	fx.finite.setDiscretization(FiniteMethods::Discretization::FORWARD_EULER);
	fx.finite.setDiffusionCoefficient(0.5);
	fx.finite.setSpaceStep(0.25);
	fx.finite.setTimeStep(0.1);
	std::vector<double> current{2.0, 1.0, 0.5, 0.0, -1.0};
	std::vector<double> next;
	fx.finite._applyDiffusion(current, next);
	ASSERT_EQ(next.size(), current.size());
	EXPECT_DOUBLE_EQ(next.front(), 2.0);
	EXPECT_DOUBLE_EQ(next.back(), -1.0);
}
