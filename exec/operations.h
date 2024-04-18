#include "../sequencia.h"

#ifndef OPERATION
#define OPERATION(name) SQValue sqop_##name(SQValue input, SQCommand children, SQValue arg)
#endif

OPERATION(split);
OPERATION(join);
OPERATION(select);
OPERATION(tokens);
OPERATION(parse);
OPERATION(str);
OPERATION(map);
OPERATION(transform);
OPERATION(len);
OPERATION(invert);
OPERATION(same);
OPERATION(any);
OPERATION(make);
OPERATION(axis);
OPERATION(filter);
OPERATION(falpha);
OPERATION(fdigit);
OPERATION(falnum);
OPERATION(contains);
OPERATION(ranges);
OPERATION(noempty);
OPERATION(append);
OPERATION(group);
OPERATION(rev);
OPERATION(use);
