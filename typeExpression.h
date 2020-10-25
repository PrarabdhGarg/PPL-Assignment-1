typedef enum {
    Integer,
    Real,
    Boolean,
    RectangularArray,
    JaggedArray
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