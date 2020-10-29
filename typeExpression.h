/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

typedef enum {
    Integer,
    Real,
    Boolean,
    RectangularArray,
    JaggedArray,
    None,
    Error
} DataTypes;

typedef struct {
    int start;
    int end;
} RectangularRange;

typedef struct {
    int size;
    int *sizes;
} JaggedRange;

typedef struct {
    int dimensions;
    RectangularRange *ranges;
    DataTypes basicElementType;
    int typeOfRange; // 0 - static 1 - dynamic
} RectangularArrayTypeExpression;

typedef struct {
    int dimensions;
    JaggedRange *ranges;
    DataTypes basicDataType;
} JaggedArrayTypeExpression;

typedef union {
    RectangularArrayTypeExpression rectangularArrayTypeExpression;
    JaggedArrayTypeExpression jaggedArrayTypeExpression;
} ArrayTypeExpression;

typedef struct {
    DataTypes type;
    ArrayTypeExpression arrayTypeExpression;
} TypeExpression;