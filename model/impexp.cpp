#include "impexp.h"

using namespace std;

/* This function creates a file named as specified in the argument 'filename' 
	containing initial data for given amount of randomly generated particles 
	
	File structure:
	a(1)	m(1)	x_0(1)	y_0(1)	v_x(1)	v_y(1)		 
	...		...		...		...		...		...
	a(N)	m(N)	x_0(N)	y_0(N)	v_x(N)	v_y(N)		 */

void generateParticles(string infoFilename, string particlesFilename, double border[4]) {
	srand(time(0));

	size_t amount;
	double minRadius;
	double maxRadius;
	double minMass;
	double maxMass;
	Vector minVelocity = Vector();
	Vector maxVelocity = Vector();
	/* Note: Vector instances here are considered more like ranges, i.e.
				abscissa has to fit the interval [ minVelocity.x, maxVelocity.x ]
				and same for the ordinate */

	/* Parameters initialization */

	ifstream fin(infoFilename);
	fin >> amount >> minRadius >> maxRadius >> minMass >> maxMass;
	fin >> minVelocity >> maxVelocity;
	fin.close();

	/* Particles generation logic */

	double currentRadius;
	double tmp;

	ofstream fout(particlesFilename);

	for (unsigned i = 0; i < amount; i++) {
		tmp = (double)rand() / RAND_MAX; //random real number in the interval [0, 1)
		currentRadius = minRadius + tmp * (maxRadius - minRadius);
		fout << currentRadius << " ";

		tmp = (double)rand() / RAND_MAX;
		fout << minMass + tmp * (maxMass - minMass) << " ";

		tmp = (double)rand() / RAND_MAX;
		fout << border[0] + currentRadius + tmp * (border[1] - border[0] - 2 * currentRadius) << " ";
		tmp = (double)rand() / RAND_MAX;
		fout << border[2] + currentRadius + tmp * (border[3] - border[2] - 2 * currentRadius) << " ";

		tmp = (double)rand() / RAND_MAX;
		fout << minVelocity.getX() + tmp * (maxVelocity.getX() - minVelocity.getX())<< " ";
		tmp = (double)rand() / RAND_MAX;
		fout << minVelocity.getY() + tmp * (maxVelocity.getY() - minVelocity.getY()) << " ";

		fout << endl;
	}
	fout.close();
}

vector<Particle> importParticles(string filename, double stiffness, double border[4]) {
	ifstream fin(filename);
	Particle p;
	vector<Particle> particle;
	Vector relativePosition;
	int peekValue;
	size_t size;

	while ((peekValue = fin.peek()) != EOF) {
		if (peekValue == ' ' || peekValue == '\n') {
			fin.get();
		}
		else {		
		fin >> p.radius >> p.mass;
		fin >> p.position >> p.velocity;
		p.stiffness = stiffness;
		particle.push_back(p);
		}
	}
	fin.close();

	size = particle.size();

	for (size_t i = 0; i < size; i++) {
		particle[i].delta = vector<double>(size);
	}

	refreshDelta(particle, border);

	return particle;
}

void exportDetails(std::string filename, const double border[4], const vector<Particle> system) {
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

void appendSystemEnergy(std::ofstream& fout, const std::vector<Particle>& system, const double border[4]) {
	auto it = system.begin();
	while (it != system.end()) {
		fout << calculateTotalEnergy(*it, border) << " ";
		it++;
	}
	fout << endl;
}