//Particle in the source system being optimized

class SourceParticle : public Particle {
public:
	SourceParticle(double x,double y,double z,double q) : Particle(x,y,z);
	virtual void getRadius() const;
	double getQ() const;
	void setQ();
protected:
	double q;
};
