
#ifndef __CPP2C__H__
#define __CPP2C__H__


struct PublicTransport
{
	struct Vtable *vptr;
	int m_license_plate;
};


struct Taxi
{
	struct PublicTransport pt;
};

struct SpecialTaxi
{
	struct Taxi taxi;
};

struct Minibus
{
	struct PublicTransport pt;
	int num_seats;
};

struct Vtable
{
	void (*dtor)(struct PublicTransport *pt);
	void (*display)(struct PublicTransport *pt);
};

struct MBVtable
{
	struct Vtable pt;
	void (*wash)(struct Minibus *mb, int minutes);
};

struct PublicConvoy
{
	struct PublicTransport pt;
	struct PublicTransport *m_pt1;
	struct PublicTransport *m_pt2;
	struct Minibus m_m;
	struct Taxi m_t;
};



void PTCtor(struct PublicTransport *const this);
void PTDtor(struct PublicTransport *const this);
void PTCCtor(struct PublicTransport *const this, const struct PublicTransport *const other);
void PTDisplay(struct PublicTransport *const this);

void TaxiCtor(struct Taxi *const this);
void TaxiDtor(struct Taxi *const this);
void TaxiDisplay(struct Taxi *const this);
void TaxiCCtor(struct Taxi *const this, const struct Taxi *other);

void SpecialTaxiCtor(struct SpecialTaxi *const this);
void SpecialTaxiDtor(struct SpecialTaxi *const this);
void SpecialTaxiDisplay(struct SpecialTaxi *const this);
void SpecialTaxiCCtor(struct SpecialTaxi *const this, const struct SpecialTaxi *other);

void MinibusCtor(struct Minibus *const this);
void MinibusDtor(struct Minibus *const this);
void MinibusDisplay(struct Minibus *const this);
void MinibusCCtor(struct Minibus *const this, const struct Minibus *other);

void PublicConvoyCtor(struct PublicConvoy *const this);
void PublicConvoyDtor(struct PublicConvoy *const this);
void PublicConvoyDisplay(struct PublicConvoy *const this);
void PublicConvoyCCtor(struct PublicConvoy *const this, const struct PublicConvoy *other);

int GetID(struct PublicTransport *pt);
void PrintCount();
void MBWash(struct Minibus *const this, int minutes);
void taxiDisplay(struct Taxi taxi);

#endif
