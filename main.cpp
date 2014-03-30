#include <iostream>
#include <stack>
using namespace std;

class cons;

union data {
    int n;
    char *s;
    cons *c;
};

const int DATA_INT = 1;
const int DATA_SYMBOL = 2;
const int DATA_STRING = 3;
const int DATA_CONS = 4;

struct obj {
    int type;
    data d;
};

struct cons {
    obj *car;
    obj *cdr;
};

static const cons *EMPTY_LIST = NULL;

obj *getPrimitive(const char *str, int &start) {
    obj *ret = new obj();
    const char *s = str + start;
    if(*s == '"') {
        ret->type = DATA_STRING;
        s++;
        start++;
    } else {
        ret->type = DATA_SYMBOL;
    }
    int length = 0;
    const char *p = s;
    for(; *s && *s!=' '; s++) {
        length++;
    }
    start += length + 1;
    if(ret->type == DATA_STRING) {
        length--;
    }
    cout << ret->type << endl;
    ret->d.s = new char[length+1];
    for(int i=0; i<length; i++) {
        ret->d.s[i] = p[i];
    }
    ret->d.s[length] = '\0';
    cout << ret->d.s << endl;
    return ret;
}

int main() {
    char str[100] = "foo \"bar\" baz";
    int start = 4;
    getPrimitive(str, start);
    getPrimitive(str, start);
    return 0;
}
