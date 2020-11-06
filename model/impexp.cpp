#include "impexp.h"

using namespace std;

void initialize(double* border, double& timeStep, bool& generatorEnabled, bool& prePacked, bool& packOnly) {
    string infoFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\info.txt)";

    ifstream fin(infoFile);
    for (int i = 0; i < 4; i++) {
        fin >> border[i];
    }

    fin >> timeStep >> generatorEnabled >> prePacked >> packOnly;
    fin.close();
}

/* This function creates a file named as specified in the argument 'particlesFilename'
	containing initial data for given amount of randomly generated particles

	File structure:
	a(1)	m(1)	x_0(1)	y_0(1)	v_x(1)	v_y(1)
	...		...		...		...		...		...
	a(N)	m(N)	x_0(N)	y_0(N)	v_x(N)	v_y(N)		 */

//void generateParticlesRandom(const string& infoFilename, const string& particlesFilename, double border[4]) {
//	srand(time(0));
//
//	size_t amount;
//	double minRadius;
//	double maxRadius;
//	double minMass;
//	double maxMass;
//	Vector minVelocity = Vector();
//	Vector maxVelocity = Vector();
//	/* Note: Vector instances here are considered more like ranges, i.e.
//				abscissa has to fit the interval [ minVelocity.x, maxVelocity.x ]
//				and same for the ordinate */
//
//	/* Parameters initialization */
//
//	ifstream fin(infoFilename);
//	fin >> amount >> minRadius >> maxRadius >> minMass >> maxMass;
//	fin >> minVelocity >> maxVelocity;
//	fin.close();
//
//	/* Particles generation logic */
//
//	double currentRadius;
//	double tmp;
//
//	ofstream fout(particlesFilename);
//
//	for (unsigned i = 0; i < amount; i++) {
//		tmp = (double)rand() / RAND_MAX; //random real number in the interval [0, 1)
//		currentRadius = minRadius + tmp * (maxRadius - minRadius);
//		fout << currentRadius << " ";
//
//		tmp = (double)rand() / RAND_MAX;
//		fout << minMass + tmp * (maxMass - minMass) << " ";
//
//		tmp = (double)rand() / RAND_MAX;
//		fout << border[0] + currentRadius + tmp * (border[1] - border[0] - 2 * currentRadius) << " ";
//		tmp = (double)rand() / RAND_MAX;
//		fout << border[2] + currentRadius + tmp * (border[3] - border[2] - 2 * currentRadius) << " ";
//
//		tmp = (double)rand() / RAND_MAX;
//		fout << minVelocity.getX() + tmp * (maxVelocity.getX() - minVelocity.getX())<< " ";
//		tmp = (double)rand() / RAND_MAX;
//		fout << minVelocity.getY() + tmp * (maxVelocity.getY() - minVelocity.getY()) << " ";
//
//		fout << endl;
//	}
//	fout.close();
//}

void generateParticlesTriangle(const string& infoFilename, const string& particlesFilename, const double border[4]) {
	srand(time(0));

	size_t amount;
	size_t N = 0;
	size_t rowCounter = 0;

	double minRadius;
	double maxRadius;
	double currentRadius;
	double tmp;

	double mass;

	double currentX;
	double currentY;
	double horizontalStep;
	double verticalStep;

	ifstream fin(infoFilename);
	fin >> amount >> minRadius >> maxRadius >> mass;
	fin.close();

	currentY = border[2] + maxRadius;
	horizontalStep = 2 * maxRadius;
	verticalStep = sqrt(3) * maxRadius;

	ofstream fout(particlesFilename);
	while (N < amount) {

		if (rowCounter % 2 == 0) {
			currentX = border[0] + maxRadius;
		}
		else {
			currentX = border[0] + 2 * maxRadius;
		}

		while (currentX < border[1] - maxRadius) {
			tmp = (double)rand() / RAND_MAX; //random real number in the interval [0, 1)
			currentRadius = minRadius + tmp * (maxRadius - minRadius);
			fout << currentRadius << " " << mass << " " << currentX << " " << currentY << " 0 0" << endl;
			N++;
			if (N >= amount) break;
			currentX += horizontalStep;
		}

		rowCounter++;
		currentY += verticalStep;
	}

	fout.close();
}

vector<Particle> importParticles(const string& sourceFile, const string& constantsFile, double border[4]) {
	ifstream fin(sourceFile);
    ifstream fin_const(constantsFile);
	Particle p;
	vector<Particle> system;
	Vector relativePosition;
	int peekValue;
	size_t size;
	double minRadius;
	double maxRadius = 0;

	while ((peekValue = fin.peek()) != EOF) {
		if (peekValue == ' ' || peekValue == '\n') {
			fin.get();
		}
		else {		
		fin >> p.radius >> p.mass;
		fin >> p.position >> p.velocity;
        minRadius = p.radius;
		if (p.radius > maxRadius) {
			maxRadius = p.radius;
		}
        if (p.radius < minRadius) {
            minRadius = p.radius;
        }

		system.push_back(p);
		}
	}
	fin.close();

	size = system.size();

	fin_const >> Particle::stiffnessRepulsive;
	fin_const >> Particle::stiffnessAttractive;
	fin_const >> Particle::stiffnessShear;
	fin_const >> Particle::frictionCoefficient;
	fin_const >> Particle::criticalDistance;

	Particle::minRadius = minRadius;
	Particle::maxRadius = maxRadius;

	return system;
}

void exportParticles(const std::string& filename, const vector<Particle>& system) {
    ofstream fout(filename);
    auto it = system.begin();
    bool wtf = fout.is_open();
    while (it != system.end()) {
        fout << it->radius << " " << it->mass << " ";
        fout << it->position << it->velocity << endl;
        it++;
    }

    fout.close();
}

void exportDetails(const std::string& filename, const double border[4], const vector<Particle>& system) {
	ofstream fout(filename);
	auto it = system.begin();

	for (int i = 0; i < 4; i++) {
		fout << border[i] << " ";
	}
	while (it != system.end()) {
		fout << it->radius << " ";
		it++;
	}

	fout.close();
}

void appendSystemPosition(std::ofstream& fout, const std::vector<Particle>& system) {
	auto it = system.begin();
	while (it != system.end()) {
		fout << it->position << " ";
		it++;
	}
	fout << endl;
}

double appendSystemEnergy(std::ofstream& fout, const std::vector<Particle>& system, std::vector<GridCell>& grid, const double border[4]) {
	auto it = system.begin();
	double systemEnergy = 0;
	double tmp;

	while (it != system.end()) {
		tmp = calculateTotalEnergy(*it, border, grid);
		systemEnergy += tmp;
		fout << tmp << " ";
		it++;
	}
	fout << endl;

	return systemEnergy;
}

double totalPositionNorm(const vector<Particle>& system) {
    double result = 0;
    auto it = system.begin();
    while (it != system.end()) {
        result += Vector::norm(it->position);
        it++;
    }
    return result;
}