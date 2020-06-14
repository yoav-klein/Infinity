class Surprizer;

Surprizer* EchoSurprize();
Surprizer* FileSurprize();
Surprizer* PrintSurprize();

class Surprizer
{
	public:
	static Surprizer* GetRandomSurprize();
	virtual void SurprizeMe();
	virtual ~Surprizer() { };
};

class File : public Surprizer
{
	virtual void SurprizeMe();	
};
	
class Echo : public Surprizer
{
	virtual void SurprizeMe();	
};

