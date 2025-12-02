/*
 * File:   FiniteMethods.cpp
 * Author: Leonardo Brito e Caio Ferreira
 */

#include "FiniteMethods.h"
#include <fstream>
#include <list>
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/SimulationControlAndResponse.h"

#ifdef PLUGINCONNECT_DYNAMIC
extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &FiniteMethods::GetPluginInformation;
}
#endif

ModelDataDefinition* FiniteMethods::NewInstance(Model* model, std::string name) {
	return new FiniteMethods(model, name);
}

std::string FiniteMethods::convertEnumToStr(DiffusionModel model) {
	switch (static_cast<int>(model)) {
		case 0: return "NONE";
		case 1: return "CONSTANT";
		default: return "UNKNOWN";
	}
}

std::string FiniteMethods::convertEnumToStr(Discretization discretization) {
	switch (static_cast<int>(discretization)) {
		case 0: return "FORWARD_EULER";
		case 1: return "CRANK_NICOLSON";
		default: return "UNKNOWN";
	}
}

std::string FiniteMethods::convertEnumToStr(Solver solver) {
	switch (static_cast<int>(solver)) {
		case 0: return "EXPLICIT";
		case 1: return "GAUSS_SEIDEL";
		default: return "UNKNOWN";
	}
}

FiniteMethods::FiniteMethods(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<FiniteMethods>(), name) {
	SimulationControlGenericClass<Variable*, Model*, Variable>* propFieldVariable = new SimulationControlGenericClass<Variable*, Model*, Variable>(
									_parentModel,
									std::bind(&FiniteMethods::getFieldVariable, this), std::bind(&FiniteMethods::setFieldVariable, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "FieldVariable", "");
	SimulationControlGenericClass<Variable*, Model*, Variable>* propTimeVariable = new SimulationControlGenericClass<Variable*, Model*, Variable>(
									_parentModel,
									std::bind(&FiniteMethods::getTimeVariable, this), std::bind(&FiniteMethods::setTimeVariable, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "TimeVariable", "");
	SimulationControlGenericEnum<FiniteMethods::DiffusionModel, FiniteMethods>* propDiffusionModel = new SimulationControlGenericEnum<FiniteMethods::DiffusionModel, FiniteMethods>(
									std::bind(&FiniteMethods::getDiffusionModel, this), std::bind(&FiniteMethods::setDiffusionModel, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "DiffusionModel", "");
	SimulationControlGenericEnum<FiniteMethods::Discretization, FiniteMethods>* propDiscretization = new SimulationControlGenericEnum<FiniteMethods::Discretization, FiniteMethods>(
									std::bind(&FiniteMethods::getDiscretization, this), std::bind(&FiniteMethods::setDiscretization, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "Discretization", "");
	SimulationControlGenericEnum<FiniteMethods::Solver, FiniteMethods>* propSolver = new SimulationControlGenericEnum<FiniteMethods::Solver, FiniteMethods>(
									std::bind(&FiniteMethods::getSolver, this), std::bind(&FiniteMethods::setSolver, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "Solver", "");
	SimulationControlGeneric<double>* propDiffusionCoefficient = new SimulationControlGeneric<double>(
									std::bind(&FiniteMethods::getDiffusionCoefficient, this), std::bind(&FiniteMethods::setDiffusionCoefficient, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "DiffusionCoefficient", "");
	SimulationControlGeneric<double>* propSpaceStep = new SimulationControlGeneric<double>(
									std::bind(&FiniteMethods::getSpaceStep, this), std::bind(&FiniteMethods::setSpaceStep, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "SpaceStep", "");
	SimulationControlGeneric<double>* propTimeStep = new SimulationControlGeneric<double>(
									std::bind(&FiniteMethods::getTimeStep, this), std::bind(&FiniteMethods::setTimeStep, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "TimeStep", "");
	SimulationControlGeneric<std::string>* propFilename = new SimulationControlGeneric<std::string>(
									std::bind(&FiniteMethods::getFilename, this), std::bind(&FiniteMethods::setFilename, this, std::placeholders::_1),
									Util::TypeOf<FiniteMethods>(), getName(), "FileName", "");

	_parentModel->getControls()->insert(propFieldVariable);
	_parentModel->getControls()->insert(propTimeVariable);
	_parentModel->getControls()->insert(propDiffusionModel);
	_parentModel->getControls()->insert(propDiscretization);
	_parentModel->getControls()->insert(propSolver);
	_parentModel->getControls()->insert(propDiffusionCoefficient);
	_parentModel->getControls()->insert(propSpaceStep);
	_parentModel->getControls()->insert(propTimeStep);
	_parentModel->getControls()->insert(propFilename);

	_addProperty(propFieldVariable);
	_addProperty(propTimeVariable);
	_addProperty(propDiffusionModel);
	_addProperty(propDiscretization);
	_addProperty(propSolver);
	_addProperty(propDiffusionCoefficient);
	_addProperty(propSpaceStep);
	_addProperty(propTimeStep);
	_addProperty(propFilename);
}

void FiniteMethods::setFieldVariable(Variable* field) {
	_fieldVariable = field;
}

Variable* FiniteMethods::getFieldVariable() const {
	return _fieldVariable;
}

void FiniteMethods::setTimeVariable(Variable* timeVariable) {
	_timeVariable = timeVariable;
}

Variable* FiniteMethods::getTimeVariable() const {
	return _timeVariable;
}

void FiniteMethods::setDiffusionModel(DiffusionModel model) {
	_diffusionModel = model;
}

FiniteMethods::DiffusionModel FiniteMethods::getDiffusionModel() const {
	return _diffusionModel;
}

void FiniteMethods::setDiscretization(Discretization discretization) {
	_discretization = discretization;
}

FiniteMethods::Discretization FiniteMethods::getDiscretization() const {
	return _discretization;
}

void FiniteMethods::setSolver(Solver solver) {
	_solver = solver;
}

FiniteMethods::Solver FiniteMethods::getSolver() const {
	return _solver;
}

void FiniteMethods::setDiffusionCoefficient(double diffusionCoefficient) {
	_diffusionCoefficient = diffusionCoefficient;
}

double FiniteMethods::getDiffusionCoefficient() const {
	return _diffusionCoefficient;
}

void FiniteMethods::setSpaceStep(double spaceStep) {
	_spaceStep = spaceStep;
}

double FiniteMethods::getSpaceStep() const {
	return _spaceStep;
}

void FiniteMethods::setTimeStep(double timeStep) {
	_timeStep = timeStep;
}

double FiniteMethods::getTimeStep() const {
	return _timeStep;
}

void FiniteMethods::setFilename(std::string filename) {
	_filename = filename;
}

std::string FiniteMethods::getFilename() const {
	return _filename;
}

std::string FiniteMethods::show() {
	return ModelComponent::show() + "";
}

ModelComponent* FiniteMethods::LoadInstance(Model* model, PersistenceRecord *fields) {
	FiniteMethods* newComponent = new FiniteMethods(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {
	}
	return newComponent;
}

bool FiniteMethods::_doStep() {
	if (_fieldVariable == nullptr || _timeVariable == nullptr) {
		return false;
	}
	double now = _parentModel->getSimulation()->getSimulatedTime();
	double currentTime = _timeVariable->getValue();
	if (currentTime + _timeStep > now + 1e-15) {
		return false;
	}
	std::list<unsigned int>* dims = _fieldVariable->getDimensionSizes();
	if (dims->empty()) {
		return false;
	}
	unsigned int points = dims->front();
	if (points < 3) {
		return false;
	}
	std::vector<double> state(points, 0.0);
	for (unsigned int i = 0; i < points; ++i) {
		state[i] = _fieldVariable->getValue(std::to_string(i));
	}
	std::vector<double> next = state;
	_applyDiffusion(state, next);
	for (unsigned int i = 0; i < points; ++i) {
		_fieldVariable->setValue(next[i], std::to_string(i));
	}
	_timeVariable->setValue(currentTime + _timeStep);
	return true;
}

void FiniteMethods::_applyDiffusion(const std::vector<double>& current, std::vector<double>& next) {
	if (_diffusionModel == DiffusionModel::NONE || current.size() < 2) {
		next = current;
		return;
	}
	double h = _spaceStep;
	double alphaDt = _diffusionCoefficient * _timeStep;
	if (_solver == Solver::EXPLICIT) {
		_applyExplicitStep(current, next, h, alphaDt);
	} else {
		double theta = (_discretization == Discretization::CRANK_NICOLSON) ? 0.5 : 1.0; // 0.5 = CN, 1.0 = backward Euler
		_applyImplicitStep(current, next, h, alphaDt, theta);
	}
	if (!next.empty()) {
		next.front() = current.front(); // Dirichlet boundaries preserved
		next.back() = current.back();
	}
}

void FiniteMethods::_applyExplicitStep(const std::vector<double>& current, std::vector<double>& next, double h, double alphaDt) const {
	next = current;
	if (current.size() < 3 || h <= 0.0) {
		return;
	}
	double invH = 1.0 / h;
	for (std::size_t i = 1; i + 1 < current.size(); ++i) {
		double mass = (i == 0 || i + 1 == current.size()) ? h * 0.5 : h; // lumped mass; boundaries unused for interior
		double laplace = (2.0 * current[i] - current[i - 1] - current[i + 1]) * invH;
		double delta = -(alphaDt / mass) * laplace;
		if (_discretization == Discretization::CRANK_NICOLSON) {
			double predictor = current[i] + delta;
			double laplacePred = (2.0 * predictor - current[i - 1] - current[i + 1]) * invH;
			double deltaPred = -(alphaDt / mass) * laplacePred;
			next[i] = current[i] + 0.5 * (delta + deltaPred);
		} else {
			next[i] = current[i] + delta;
		}
	}
}

void FiniteMethods::_applyImplicitStep(const std::vector<double>& current, std::vector<double>& next, double h, double alphaDt, double theta) const {
	std::size_t n = current.size();
	next.assign(n, 0.0);
	if (n < 3 || h <= 0.0) {
		next = current;
		return;
	}
	double invH = 1.0 / h;
	double kMain = 2.0 * invH;
	double kOff = -1.0 * invH;
	double factor = theta * alphaDt;
	double factorExp = (1.0 - theta) * alphaDt;

	std::vector<double> diag(n, 0.0);
	std::vector<double> lower(n > 0 ? n - 1 : 0, 0.0);
	std::vector<double> upper(n > 0 ? n - 1 : 0, 0.0);
	std::vector<double> rhs(n, 0.0);

	// boundary rows (Dirichlet)
	diag.front() = 1.0;
	rhs.front() = current.front();
	diag.back() = 1.0;
	rhs.back() = current.back();

	for (std::size_t i = 1; i + 1 < n; ++i) {
		double mass = h; // lumped mass for interior
		double ku = kMain * current[i] + kOff * current[i - 1] + kOff * current[i + 1];
		double rhsVal = mass * current[i] - factorExp * ku;

		double a = factor * kOff;
		double b = mass + factor * kMain;
		double c = factor * kOff;

		lower[i - 1] = a;
		diag[i] = b;
		upper[i] = c;
		rhs[i] = rhsVal;
	}

	// Incorporate boundary values into RHS for Dirichlet conditions
	double leftBoundary = current.front();
	double rightBoundary = current.back();
	if (n >= 2) {
		rhs[1] -= lower[0] * leftBoundary;
		lower[0] = 0.0;
		rhs[n - 2] -= upper[n - 2] * rightBoundary;
		upper[n - 2] = 0.0;
	}

	_solveTridiagonal(lower, diag, upper, rhs);
	next = rhs;
}

void FiniteMethods::_solveTridiagonal(const std::vector<double>& lower, const std::vector<double>& diag, const std::vector<double>& upper, std::vector<double>& rhs) const {
	std::size_t n = rhs.size();
	if (n == 0) return;
	std::vector<double> cPrime(n, 0.0);
	std::vector<double> dPrime(n, 0.0);

	cPrime[0] = (n > 1) ? upper[0] / diag[0] : 0.0;
	dPrime[0] = rhs[0] / diag[0];

	for (std::size_t i = 1; i < n; ++i) {
		double m = diag[i] - lower[i - 1] * cPrime[i - 1];
		double c = (i < n - 1) ? upper[i] / m : 0.0;
		double d = (rhs[i] - lower[i - 1] * dPrime[i - 1]) / m;
		cPrime[i] = c;
		dPrime[i] = d;
	}

	rhs[n - 1] = dPrime[n - 1];
	for (std::size_t i = n - 1; i-- > 0; ) {
		rhs[i] = dPrime[i] - cPrime[i] * rhs[i + 1];
	}
}

void FiniteMethods::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
	std::ofstream savefile;
	if (_filename != "") {
		savefile.open(_filename, std::ofstream::app);
	}
	while (_doStep()) {
		if (_fieldVariable != nullptr) {
			std::string message = "t=" + std::to_string(_timeVariable->getValue());
			for (unsigned int i = 0; i < _fieldVariable->getDimensionSizes()->front(); ++i) {
				message += " ," + _fieldVariable->getName() + "[" + std::to_string(i) + "]=" + std::to_string(_fieldVariable->getValue(std::to_string(i)));
			}
			traceSimulation(this, message, TraceManager::Level::L8_detailed);
			if (savefile.is_open()) {
				message = std::to_string(_timeVariable->getValue());
				for (unsigned int i = 0; i < _fieldVariable->getDimensionSizes()->front(); ++i) {
					message += "\t" + std::to_string(_fieldVariable->getValue(std::to_string(i)));
				}
				savefile << message << std::endl;
			}
		}
	}
	if (savefile.is_open()) {
		savefile.close();
	}
	_parentModel->sendEntityToComponent(entity, getConnectionManager()->getFrontConnection());
}

bool FiniteMethods::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		_diffusionModel = static_cast<DiffusionModel>(fields->loadField("diffusionModel", static_cast<int>(DEFAULT.diffusionModel)));
		_discretization = static_cast<Discretization>(fields->loadField("discretization", static_cast<int>(DEFAULT.discretization)));
		_solver = static_cast<Solver>(fields->loadField("solver", static_cast<int>(DEFAULT.solver)));
		_diffusionCoefficient = fields->loadField("diffusionCoefficient", DEFAULT.diffusionCoefficient);
		_spaceStep = fields->loadField("spaceStep", DEFAULT.spaceStep);
		_timeStep = fields->loadField("timeStep", DEFAULT.timeStep);
		_filename = fields->loadField("filename", DEFAULT.filename);
	}
	return res;
}

void FiniteMethods::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
	fields->saveField("diffusionModel", static_cast<int>(_diffusionModel), static_cast<int>(DEFAULT.diffusionModel), saveDefaultValues);
	fields->saveField("discretization", static_cast<int>(_discretization), static_cast<int>(DEFAULT.discretization), saveDefaultValues);
	fields->saveField("solver", static_cast<int>(_solver), static_cast<int>(DEFAULT.solver), saveDefaultValues);
	fields->saveField("diffusionCoefficient", _diffusionCoefficient, DEFAULT.diffusionCoefficient, saveDefaultValues);
	fields->saveField("spaceStep", _spaceStep, DEFAULT.spaceStep, saveDefaultValues);
	fields->saveField("timeStep", _timeStep, DEFAULT.timeStep, saveDefaultValues);
	fields->saveField("filename", _filename, DEFAULT.filename, saveDefaultValues);
}

bool FiniteMethods::_check(std::string& errorMessage) {
	bool ok = true;
	if (_fieldVariable == nullptr) {
		errorMessage += "FieldVariable not set; ";
		ok = false;
	}
	if (_timeVariable == nullptr) {
		errorMessage += "TimeVariable not set; ";
		ok = false;
	}
	if (_spaceStep <= 0.0) {
		errorMessage += "SpaceStep must be positive; ";
		ok = false;
	}
	if (_timeStep <= 0.0) {
		errorMessage += "TimeStep must be positive; ";
		ok = false;
	}
	if (_diffusionModel == DiffusionModel::CONSTANT && _diffusionCoefficient < 0.0) {
		errorMessage += "DiffusionCoefficient must be non-negative; ";
		ok = false;
	}
	if (ok && _fieldVariable != nullptr && _fieldVariable->getDimensionSizes()->empty()) {
		errorMessage += "FieldVariable dimension not set; ";
		ok = false;
	}
	if (ok && _fieldVariable != nullptr && _fieldVariable->getDimensionSizes()->front() < 3) {
		errorMessage += "FieldVariable requires at least 3 points; ";
		ok = false;
	}
	if (ok && _filename != "") {
		try {
			std::ofstream savefile(_filename, std::ofstream::out);
			std::string header = _timeVariable != nullptr ? _timeVariable->getName() : "t";
			if (_fieldVariable != nullptr) {
				for (unsigned int i = 0; i < _fieldVariable->getDimensionSizes()->front(); ++i) {
					header += "\t" + _fieldVariable->getName() + "[" + std::to_string(i) + "]";
				}
			}
			savefile << header << std::endl;
			savefile.close();
		} catch (...) {
			errorMessage += "Could not open file for FiniteMethods output; ";
			ok = false;
		}
	}
	return ok;
}

PluginInformation* FiniteMethods::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<FiniteMethods>(), &FiniteMethods::LoadInstance, &FiniteMethods::NewInstance);
	info->setCategory("Continuous");
	return info;
}
