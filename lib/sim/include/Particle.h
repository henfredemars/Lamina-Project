//Superclass of LaminaParticle and SourceParticle

class Particle {

public:
	Particle(double x,double y, double z);
	double getX() const;
	double getY() const;
	double getZ() const;
	virtual double getRadius() const;
	double distanceTo(Particle p) const;
protected:
	double x;
	double y;
	double z;
};
