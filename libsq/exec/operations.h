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
OPERATION(contains);
OPERATION(ranges);
OPERATION(noempty);
OPERATION(append);
OPERATION(prepend);
OPERATION(group);
OPERATION(rev);
OPERATION(use);
OPERATION(run);
OPERATION(with);
OPERATION(flatten);
OPERATION(deshape);
OPERATION(flatmap);
OPERATION(tolower);
OPERATION(toupper);

OPERATION(falpha);
OPERATION(fdigit);
OPERATION(falnum);
OPERATION(fpunct);
OPERATION(fwhite);
OPERATION(fnalpha);
OPERATION(fndigit);
OPERATION(fnalnum);
OPERATION(fnpunct);
OPERATION(fnwhite);
