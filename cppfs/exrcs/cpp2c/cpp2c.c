#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "cpp2c.h"

#define MAX(a, b) ((a) > (b) ? (int)a : (int)b)

int s_count = 0;


/* vtable instances */
struct Vtable pt_vtable = { PTDtor, PTDisplay };
struct Vtable taxi_vtable = { (void (*)(struct PublicTransport*))TaxiDtor, 
							  (void (*)(struct PublicTransport*))TaxiDisplay };

struct Vtable staxi_vtable = { (void (*)(struct PublicTransport*))SpecialTaxiDtor,
								(void (*)(struct PublicTransport*))SpecialTaxiDisplay };
								
struct MBVtable mb_vtable = { { (void (*)(struct PublicTransport*))MinibusDtor,
								(void (*)(struct PublicTransport*))MinibusDisplay }, 
								MBWash }; 

struct Vtable convoy_vtable = { (void (*)(struct PublicTransport*))PublicConvoyDtor,
								(void (*)(struct PublicTransport*))PublicConvoyDisplay };

/* PublicTransport */
void PTCtor(struct PublicTransport *const this)
{
	this->vptr = &pt_vtable;
	++s_count;
	this->m_license_plate = s_count;
	
	printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void PTCCtor(struct PublicTransport *const this, const struct PublicTransport *const other)
{
	(void)other;
	this->vptr = &pt_vtable;
	++s_count;
	this->m_license_plate = s_count;
	
	printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void PTDtor(struct PublicTransport *const this)
{
	--s_count;
	printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void PTDisplay(struct PublicTransport *const this)
{
	printf("PublicTransport::display(): %d\n", this->m_license_plate);
}	


/* Taxi */
void TaxiCtor(struct Taxi *const this)
{
	PTCtor(&this->pt);
	this->pt.vptr = &taxi_vtable;
	printf("Taxi::Ctor()\n");
}

	printf("SpecialTaxi::Dtor()\n");
	TaxiDtor(&this->taxi);
}

void SpecialTaxiDisplay(struct SpecialTaxi *const this)
{
	printf("SpecialTaxi::display() ID:%d\n", this->taxi.pt.m_license_plate);
}

void SpecialTaxiCCtor(struct SpecialTaxi *const this, const struct SpecialTaxi *other)
{
	TaxiCCtor(&this->taxi, &other->taxi);
	this->taxi.pt.vptr = &staxi_vtable;
	printf("SpecialTaxi::CCtor()\n");
}

int GetID(struct PublicTransport *pt)
{
	return pt->m_license_plate;
}

void PrintCount()
{
	printf("s_count: %d\n", s_count);
}

/* Minibus */
void MinibusCtor(struct Minibus *const this)
{
	PTCtor(&this->pt);
	this->pt.vptr = (struct Vtable*)&mb_vtable;
	
	this->num_seats = 20;
	
	printf("Minibus::Ctor()\n");
}

void MinibusDtor(struct Minibus *const this)
{
	printf("Minibus::Dtor()\n");
	PTDtor(&this->pt);
}

void MinibusDisplay(struct Minibus *const this)
{
	printf("Minibus::display() ID:%d num seats:%d\n", this->pt.m_license_plate, this->num_seats);
}

void MinibusCCtor(struct Minibus *const this, const struct Minibus *other)
{
	PTCCtor(&this->pt, &other->pt);
	this->pt.vptr = (struct Vtable*)&mb_vtable;
	this->num_seats = other->num_seats;
	printf("Minibus::CCtor()\n");
}

void MBWash(struct Minibus *const this, int minutes)
{
	printf("Minibus::wash(%d) ID:%d\n", minutes, this->pt.m_license_plate);
}

/* Convoy */
void PublicConvoyCtor(struct PublicConvoy *const this)
{
	PTCtor(&this->pt);
	this->pt.vptr = &convoy_vtable;
	
	this->m_pt1 = malloc(sizeof(struct Minibus));
	this->m_pt2 = malloc(sizeof(struct Taxi));
	
	MinibusCtor((struct Minibus*)this->m_pt1);
	TaxiCtor((struct Taxi*)this->m_pt2);

	MinibusCtor(&this->m_m);
	TaxiCtor(&this->m_t);
}

void PublicConvoyDtor(struct PublicConvoy *const this)
{
	MinibusDtor((struct Minibus*)this->m_pt1);
	free(this->m_pt1);
	TaxiDtor((struct Taxi*)this->m_pt2);
	free(this->m_pt2);
	TaxiDtor(&this->m_t);
	MinibusDtor(&this->m_m);
	
	PTDtor(&this->pt);
}

void PublicConvoyDisplay(struct PublicConvoy *const this)
{
	MinibusDisplay((struct Minibus*)this->m_pt1);
	TaxiDisplay((struct Taxi*)this->m_pt2);
	MinibusDisplay((struct Minibus*)&this->m_m);
	TaxiDisplay((struct Taxi*)&this->m_t);
}

void PublicConvoyCCtor(struct PublicConvoy *const this, const struct PublicConvoy *other)
{
	PTCCtor(&this->pt, &other->pt);
	
	this->m_pt1 = malloc(sizeof(struct Minibus));
	this->m_pt2 = malloc(sizeof(struct Taxi));
	
	MinibusCCtor((struct Minibus*)this->m_pt1, (struct Minibus*)other->m_pt1);
	TaxiCCtor((struct Taxi*)this->m_pt2, (struct Taxi*)other->m_pt2); 
	MinibusCCtor((struct Minibus*)&this->m_m, (struct Minibus*)&other->m_m);
	TaxiCCtor((struct Taxi*)&this->m_t, (struct Taxi*)&other->m_t);
	
	this->pt.vptr = &convoy_vtable;
}

/*--- others --*/

void taxiDisplay(struct Taxi taxi)
{
	
	taxi.pt.vptr->display((struct PublicTransport*)&taxi);
}

void PrintInfoMinibus(struct Minibus* mb)
{
	MBWash(mb, 3);
}

struct PublicTransport PrintInfoInt(int i)
{
	struct Minibus mb;
	struct PublicTransport ret;
	
	MinibusCtor(&mb);
	(void)i;
	printf("print_info(int i)\n");
	MinibusDisplay(&mb);
	PTCCtor(&ret, &mb.pt);
	MinibusDtor(&mb);
	
	return ret;
}

void PrintInfoPT(struct PublicTransport *pt)
{
	pt->vptr->display(pt);
}

int main()
{
	int i = 0;
	struct Minibus a;
	struct Minibus m;
	struct Minibus m2;
	struct PublicTransport ret;
	struct PublicTransport* array[3];
	struct PublicTransport array_value[3];
	struct Taxi taxi;
	struct Minibus mb_arr[4];
	struct Taxi *taxi_arr[4];
	struct SpecialTaxi st;
	struct PublicConvoy *st1;
	struct PublicConvoy *st2;
	struct Minibus temp;
	
	MinibusCtor(&a);
	MinibusCtor(&m);
	
	PrintInfoMinibus(&m);
	
	ret = PrintInfoInt(3);
	PTDisplay(&ret);
	PTDtor(&ret);
	
	array[0] = malloc(sizeof(struct Minibus));
	array[1] = malloc(sizeof(struct Taxi));
	array[2] = malloc(sizeof(struct Minibus));
	
	MinibusCtor((struct Minibus*)array[0]);
	TaxiCtor((struct Taxi*)array[1]);
	MinibusCtor((struct Minibus*)array[2]);
	
	for(i = 0; i < 3; ++i)
	{
		array[i]->vptr->display(array[i]);
	}
	
	for(i = 0; i < 3; ++i)
	{
		array[i]->vptr->dtor(array[i]);
		free(array[i]);
	}
	
	MinibusCtor(&temp);
	PTCCtor(&array_value[0], &temp.pt);
	MinibusDtor(&temp);
	
	TaxiCtor(&taxi);
	PTCCtor(&array_value[1], &taxi.pt);
	TaxiDtor(&taxi);
	
	PTCtor(&array_value[2]);
	
	for(i = 0; i < 3; ++i)
	{
		array_value[i].vptr->display(&array_value[i]);
	}
	
	PrintInfoPT(&array_value[0]);
	
	PrintCount();
	
	MinibusCtor(&m2);
	
	PrintCount();
	
	for(i = 0; i < 4; ++i)
	{
		MinibusCtor(&mb_arr[i]);
	}
	
	for(i = 0; i < 4; ++i)
	{
		taxi_arr[i] = malloc(sizeof(struct Taxi));
		TaxiCtor(taxi_arr[i]);
	}	
	
	for(i = 3; i >= 0; --i)
	{
		TaxiDtor(taxi_arr[i]);
		free(taxi_arr[i]);
	}
	
	printf("%d\n%d\n", MAX(1, 2), MAX(1, 2.0f));
	
	SpecialTaxiCtor(&st);
	
	TaxiCCtor(&taxi, &st.taxi);
	taxiDisplay(taxi);
	TaxiDtor(&taxi);
	
	st1 = malloc(sizeof(struct PublicConvoy));
	PublicConvoyCtor(st1);
	
	st2 = malloc(sizeof(struct PublicConvoy));
	PublicConvoyCCtor(st2, st1);
	
	st1->pt.vptr->display(&st1->pt);
	st2->pt.vptr->display(&st2->pt);
	
	PublicConvoyDtor(st1);
	free(st1);
	
	st2->pt.vptr->display(&st2->pt);
	
	PublicConvoyDtor(st2);
	free(st2);
	
	/* Destruct locals */
	SpecialTaxiDtor(&st);
	
	for(i = 3; i >=0; --i)
	{
		MinibusDtor(&mb_arr[i]);
	}
	
	MinibusDtor(&m2);
	
	for(i = 2; i >=0; --i)
	{
		array_value[i].vptr->dtor(&array_value[i]);
	}
	
	MinibusDtor(&m);
	MinibusDtor(&a);
	
	return 0;
}
