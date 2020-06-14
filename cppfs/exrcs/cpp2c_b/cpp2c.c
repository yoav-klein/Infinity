#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#define MAX(a, b) (a) > (b) ? a : b

static int g_count = 0;

/******* Animals structs **********/
struct Animal
{
	struct AnimalVTable *vptr;
	int id;
};

struct Centipede
{
	struct Animal animal;
	int num_legs;
};

struct Squirl
{
	struct Animal animal;
};

struct FlySquirl
{
	struct Squirl squirl;
};

struct LegendaryAnimal
{	
	struct Animal animal;
	struct Animal *a1;
	struct Animal *a2;
	struct Centipede c;
	struct Squirl sq;
};

/****** VTable structs *********/

struct AnimalVTable 
{	
	void (*dtor)(struct Animal *const this);
	void (*display)(struct Animal *const this);
};

struct CentVTable
{
	struct AnimalVTable animal_vtbl;
	void (*buy_shoes)(struct Centipede *const this);
};

struct SquirlVTable
{
	struct AnimalVTable animal_vtbl;
};

struct FlySquarleVTable
{
	struct AnimalVTable animal_vtbl;
};

struct LegendaryAnimalVTable
{
	struct AnimalVTable animal_vtbl;
};

/************* Function Declerations **************/

/** Animal **/
void AnimalCtor(struct Animal *const this);
void AnimalDtor(struct Animal *const this);
void AnimalCCtor(struct Animal *const this, const struct Animal *other);
void AnimalDisplay(struct Animal *const this);
int GetID(struct Animal *const this);

/** Centipede **/
void CentipedeCtor(struct Centipede *const this);
void CentipedeDtor(struct Centipede *const this);
void CentipedeCCtor(struct Centipede *const this, const struct Centipede *other);
void CentipedeDisplay(struct Centipede *const this);
void CentipedeBuyShoes(struct Centipede *const this);

/** Squirl **/
void SquirlCtor(struct Squirl *const this);
void SquirlDtor(struct Squirl *const this);
void SquirlCCtor(struct Squirl *const this, const struct Squirl *other);
void SquirlDisplay(struct Squirl *const this);

/** FlyingSquirl **/
void FlySquirlCtor(struct FlySquirl *const this);
void FlySquirlDtor(struct FlySquirl *const this);
void FlySquirlCCtor(struct FlySquirl *const this, const struct FlySquirl *other);
void FlySquirlDisplay(struct FlySquirl *const this);

/** LegendaryAnimal **/
void LegendaryAnimalCtor(struct LegendaryAnimal *const this);
void LegendaryAnimalDtor(struct LegendaryAnimal *const this);
void LegendaryAnimalCCtor(struct LegendaryAnimal *const this, 
									const struct LegendaryAnimal *other);
void LegendaryAnimalDisplay(struct LegendaryAnimal *const this);

/** others **/
void PrintCount();
void FooA(struct Animal *animal);
void FooV();
struct Animal FooI(int i);
void Fifi(struct Squirl s);


/***** vtables **********/
struct AnimalVTable animal_vtbl = { AnimalDtor, AnimalDisplay };
struct CentVTable cent_vtbl = { 
				{ (void(*)(struct Animal *const))CentipedeDtor,  
				  (void(*)(struct Animal *const))CentipedeDisplay  },
				   CentipedeBuyShoes  };
struct SquirlVTable squirl_vtbl = { 
				{ (void(*)(struct Animal *const))SquirlDtor,  
				  (void(*)(struct Animal *const))SquirlDisplay  }  };

struct FlySquarleVTable fly_squirl_vtbl = { 
				{ (void(*)(struct Animal *const))FlySquirlDtor,  
				  (void(*)(struct Animal *const))FlySquirlDisplay  }  };

struct LegendaryAnimalVTable leg_animal_vtbl = { 
				{ (void(*)(struct Animal *const))LegendaryAnimalDtor,  
				  (void(*)(struct Animal *const))LegendaryAnimalDisplay  }  };



/*************** Function Impls *******************/

/** Animal **/
void AnimalCtor(struct Animal *const this)
{
	this->vptr = &animal_vtbl;
	this->id = ++g_count;

	printf("Animal::Ctor()%d\n", this->id);
}

void AnimalDtor(struct Animal *const this)
{
	--g_count;
	printf("Animal::Dtor()%d\n", this->id);
}

void AnimalCCtor(struct Animal *const this, const struct Animal *other)
{
	(void)other;
	this->id = ++g_count;
	printf("Animal::CCtor() %d\n", this->id);
}

void AnimalDisplay(struct Animal *const this)
{
	printf("Animal::Display() : %d\n", this->id);
}

int GetID(struct Animal *const this)
{
	return this->id;
}

/** Centipede **/
void CentipedeCtor(struct Centipede *const this)
{
	AnimalCtor(&this->animal);
	
	this->animal.vptr = (struct AnimalVTable*)&cent_vtbl;
	this->num_legs = 46;
	
	printf("Centipede::Ctor()\n");
}

void CentipedeDtor(struct Centipede *const this)
{
	printf("Centipede::dtor()\n");
	AnimalDtor((struct Animal*)this);
}

void CentipedeCCtor(struct Centipede *const this, const struct Centipede *other)
{
	AnimalCCtor((struct Animal*)this, (struct Animal*)other);
	this->animal.vptr = (struct AnimalVTable*)&cent_vtbl;
	this->num_legs = other->num_legs;
	
	printf("Centipede::CCtor()\n");
}

void CentipedeDisplay(struct Centipede *const this)
{
	printf("Centipede::Display() ID:%d  num legs:%d\n", 
			GetID((struct Animal*)this), this->num_legs);
}

void CentipedeBuyShoes(struct Centipede *const this)
{
	printf("Centipede::BuyShues() ID:%d\n", GetID((struct Animal*)this));
}

/** Squirl **/
void SquirlCtor(struct Squirl *const this)
{
	AnimalCtor(&this->animal);
	
	this->animal.vptr = (struct AnimalVTable*)&squirl_vtbl;
	
	printf("Squirl::Ctor()\n");
}

void SquirlDtor(struct Squirl *const this)
{
	printf("Squirl::dtor()\n");
	AnimalDtor((struct Animal*)this);
}

void SquirlCCtor(struct Squirl *const this, const struct Squirl *other)
{
	AnimalCCtor((struct Animal*)this, (struct Animal*)other);
	this->animal.vptr = (struct AnimalVTable*)&squirl_vtbl;
	
	printf("Squirl::CCtor()\n");
}

void SquirlDisplay(struct Squirl *const this)
{
	printf("Squirl::Display() ID:%d\n", GetID((struct Animal*)this));
}

/** FlyingSquirl **/
void FlySquirlCtor(struct FlySquirl *const this)
{
	SquirlCtor(&this->squirl);
	
	this->squirl.animal.vptr = (struct AnimalVTable*)&fly_squirl_vtbl;
	
	printf("FlyingSquirl::Ctor()\n");
}

void FlySquirlDtor(struct FlySquirl *const this)
{
	printf("FlyingSquirl::dtor()\n");
	SquirlDtor(&this->squirl);
}

void FlySquirlCCtor(struct FlySquirl *const this, const struct FlySquirl *other)
{
	AnimalCCtor((struct Animal*)this, (struct Animal*)other);
	this->squirl.animal.vptr = (struct AnimalVTable*)&fly_squirl_vtbl;
	
	printf("FlyingSquirl::CCtor()\n");
}

void FlySquirlDisplay(struct FlySquirl *const this)
{
	printf("FlyingSquirl::Display() ID:%d\n", GetID((struct Animal*)this));
}

/** Legendary Animal **/
void LegendaryAnimalCtor(struct LegendaryAnimal *const this)
{
	AnimalCtor(&this->animal);
		
	this->animal.vptr = (struct AnimalVTable*)&leg_animal_vtbl;
	
	this->a1 = malloc(sizeof(struct Centipede));
	this->a2 = malloc(sizeof(struct Squirl));
	CentipedeCtor((struct Centipede*)this->a1);
	SquirlCtor((struct Squirl*)this->a2);
	
	CentipedeCtor(&this->c);
	SquirlCtor(&this->sq);
}

void LegendaryAnimalDtor(struct LegendaryAnimal *const this)
{
	CentipedeDtor((struct Centipede*)this->a1);
	SquirlDtor((struct Squirl*)this->a2);
	
	free(this->a1);
	free(this->a2);
	
	SquirlDtor(&this->sq);
	CentipedeDtor(&this->c);
	
	AnimalDtor(&this->animal);
}

void LegendaryAnimalCCtor(struct LegendaryAnimal *const this, 
									const struct LegendaryAnimal *other)
{
	AnimalCCtor(&this->animal, &other->animal);
	
	this->a1 = malloc(sizeof(struct Centipede));
	this->a2 = malloc(sizeof(struct Squirl));
	
	CentipedeCCtor((struct Centipede*)this->a1, (struct Centipede*)other->a1);
	SquirlCCtor((struct Squirl*)this->a2, (struct Squirl*)other->a2);
	
	CentipedeCCtor(&this->c, &other->c);
	SquirlCCtor(&this->sq, &other->sq);
	
	this->animal.vptr = (struct AnimalVTable*)&leg_animal_vtbl;
}

void LegendaryAnimalDisplay(struct LegendaryAnimal *const this)
{
	this->a1->vptr->display(this->a1);
	this->a2->vptr->display(this->a2);
	CentipedeDisplay(&this->c);
	SquirlDisplay(&this->sq);
}

/** others **/
void FooA(struct Animal *animal)
{
	animal->vptr->display(animal);
}

void Foo()
{
	PrintCount();
}

void PrintCount()
{
	printf("s_count: %d\n", g_count);
}

struct Animal FooI(int i)
{
	struct Centipede c;
	struct Animal temp;
	
	(void)i;
	CentipedeCtor(&c);
	
	printf("Foo(int i)\n");
	CentipedeDisplay(&c);
	
	AnimalCCtor(&temp, &c.animal);
	CentipedeDtor(&c);
	
	return temp;
}

void Fifi(struct Squirl s)
{
	SquirlDisplay(&s);
}

void Origin()
{
	struct Centipede c; /* 1 local */
	struct Animal ret_from_foo;
	struct Animal *array[3];
	struct Animal arr2[3]; /* 2 local */
	
	struct Centipede temp_cent; 
	struct Squirl temp_squirl;
	
	struct Centipede c2; /* 3 local */
	struct Centipede arr3[4]; /* 4 local*/
	
	struct FlySquirl fs; /* 5 local */
	
	struct Squirl *arr4[4];
	
	struct LegendaryAnimal *la1;
	struct LegendaryAnimal *la2;
	
	int i = 0;
	
	CentipedeCtor(&c);
	FooA(&c.animal);	
	
	ret_from_foo = FooI(3); 
	AnimalDisplay(&ret_from_foo);
	AnimalDtor(&ret_from_foo);
	
	array[0] = malloc(sizeof(struct Centipede));
	array[1] = malloc(sizeof(struct Squirl));
	array[2] = malloc(sizeof(struct Centipede));
	
	CentipedeCtor((struct Centipede*)array[0]);
	SquirlCtor((struct Squirl*)array[1]);
	CentipedeCtor((struct Centipede*)array[2]);
	
	for(i = 0; i < 3; ++i)
	{
		((struct Animal*)array[i])->vptr->display((struct Animal*)array[i]);
	}
	
	for(i = 0; i <3; ++i)
	{
		((struct Animal*)array[i])->vptr->dtor((struct Animal*)array[i]);
		free(array[i]);
	}
	CentipedeCtor(&temp_cent);
	AnimalCCtor(&arr2[0], &temp_cent.animal);
	CentipedeDtor(&temp_cent);
	
	SquirlCtor(&temp_squirl);
	AnimalCCtor(&arr2[1], &temp_squirl.animal);
	SquirlDtor(&temp_squirl);
	
	AnimalCtor(&arr2[2]);
	
	for(i = 0; i < 3; ++i)
	{
		AnimalDisplay(&arr2[i]);
	}
	
	PrintCount();
	
	CentipedeCtor(&c2);
	PrintCount();
		
	for(i = 0; i < 4; ++i)
	{
		CentipedeCtor(&arr3[i]);
	}
	
	for(i = 0; i < 4; ++i)
	{
		arr4[i] = malloc(sizeof(struct Squirl));
		SquirlCtor(arr4[i]);
	}
	
	for(i = 3; i >= 0; --i)
	{
		SquirlDtor(arr4[i]);
		free(arr4[i]);
	}
	
	printf("%d\n", MAX(1, 2));
	FlySquirlCtor(&fs);
	
	SquirlCCtor(&temp_squirl, &(fs.squirl));
	Fifi(temp_squirl);
	SquirlDtor(&temp_squirl);
	
	la1 = malloc(sizeof(struct LegendaryAnimal));
	la2 = malloc(sizeof(struct LegendaryAnimal));
	
	LegendaryAnimalCtor(la1);
	LegendaryAnimalCCtor(la2, la1);
	
	LegendaryAnimalDisplay(la1);
	LegendaryAnimalDisplay(la2);
	
	LegendaryAnimalDtor(la1);
	free(la1);
	
	LegendaryAnimalDisplay(la2);	
	
	
	LegendaryAnimalDtor(la2);
	free(la2);
	
	FlySquirlDtor(&fs);
	
	for(i = 3; i >= 0; --i)
	{
		CentipedeDtor(&arr3[i]);
	}
	
	CentipedeDtor(&c2);
	
	for(i = 2; i >= 0; --i)
	{
		AnimalDtor(&arr2[i]);
	}
	
	CentipedeDtor(&c);
}

int main()
{
	struct Animal *a = malloc(sizeof(struct Centipede));
	
	CentipedeCtor((struct Centipede*)a);
	
	a->vptr->display(a);
	return 0;

}




